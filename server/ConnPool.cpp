#include "ConnPool.h"
#include <fstream>
#include <thread>
#include <chrono>
using namespace std;

ConnPool::ConnPool()
{
	ip = "123.60.157.65";
	user = "root";
	passwd = "87047798abAB@";
	db = "huanuo";
	port = 3306;
	min = 20;
	max = 100;
	timeout = 1000;
	idletime = 5000;
	for (int i = 0; i < min; i++)
	{
		addConn();
	}
	thread producer(&ConnPool::produce, this);
	thread recycler(&ConnPool::recycle, this);
	producer.detach();
	recycler.detach();
}

ConnPool::~ConnPool()
{
	while (!connq.empty())
	{
		auto conn = connq.front();
		connq.pop();
		delete conn;
	}
}

void ConnPool::produce()
{
	while (1)
	{
		unique_lock<mutex> lock(mtx);
		while (connq.size() >= min)
		{
			cv.wait(lock);
		}
		addConn();
		cv.notify_all();
	}
}

void ConnPool::recycle() // 回收线程的逻辑不允许回收太多，故不用唤醒生产者线程
{
	while (1)
	{
		this_thread::sleep_for(chrono::seconds(1));
		unique_lock<mutex> lock(mtx);
		while (connq.size() > min)
		{
			MysqlConn* conn = connq.front();
			if (conn->getAliveTime() > idletime || connq.size() > max)
			{
				connq.pop();
				delete conn;
			}
			else
			{
				break;
			}
		}
	}
}

void ConnPool::addConn()
{
	MysqlConn* conn = new MysqlConn;
	conn->connect(ip, port, user, passwd, db);
	conn->refreshTime();
	connq.push(conn);
}

shared_ptr<MysqlConn> ConnPool::get()
{
	unique_lock<mutex> lock(mtx);
	while (connq.empty())
	{
		cv.wait(lock);
	}
	shared_ptr<MysqlConn> conn(connq.front(), [this](MysqlConn* mc)
		{
			unique_lock<mutex> lock2(mtx);
			connq.push(mc);
			mc->refreshTime();
		});
	connq.pop();
	lock.unlock();
	cv.notify_all();
	return conn;
}

ConnPool* ConnPool::getInstance()
{
	return CP;
}

ConnPool* ConnPool::CP = new ConnPool;