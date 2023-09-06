#include "MysqlConn.h"
#include <string>
#include <mysql.h>
#include <iostream>
#include <chrono>
using namespace chrono;
using namespace std;

MysqlConn::MysqlConn()
{
	conn = mysql_init(nullptr);
	mysql_set_character_set(conn, "utf8");
	mysql_autocommit(conn, false);
}

MysqlConn::~MysqlConn()
{
	if (res)
	{
		mysql_free_result(res);
		res = nullptr;
	}
	if (conn)
	{
		mysql_close(conn);
	}
}

bool MysqlConn::connect(string ip, unsigned short port, string user, string passwd, string db)
{
	auto ptr = mysql_real_connect(conn, ip.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, nullptr, 0);
	return ptr != nullptr;
}

bool MysqlConn::update(string sql)
{
	auto r = mysql_query(conn, sql.c_str());
	if (r == 0) mysql_commit(conn);
	else mysql_rollback(conn);
	return r == 0;
}

bool MysqlConn::query(string sql)
{
	if (res)
	{
		mysql_free_result(res);
		res = nullptr;
	}
	auto r = mysql_query(conn, sql.c_str());
	if (r != 0) return false;
	res = mysql_store_result(conn);
	return true;
}

shared_ptr<Row> MysqlConn::next()
{
	if (!res) return nullptr;
	shared_ptr<Row> row(new Row);
	row->data = mysql_fetch_row(res);
	if (!row->data) return nullptr;
	row->cnt = mysql_num_fields(res);
	return row;
}

template<typename... Args>
bool MysqlConn::transaction(string sql, Args... args)
{
	if (mysql_query(conn, sql.c_str()) != 0)
	{
		mysql_rollback(conn);
		return false;
	}
	return transaction(args...);
}

bool MysqlConn::transaction()
{
	mysql_commit(conn);
	return true;
}

long long MysqlConn::getAliveTime()
{
	auto r = steady_clock::now() - active; // Ĭ�Ͼ�ȷ������
	auto res = duration_cast<milliseconds>(r); // ת��������
	return res.count(); // ���ع��ж��ٺ���
}

void MysqlConn::refreshTime()
{
	if (res)
	{
		mysql_free_result(res);
		res = nullptr;
	}
	active = steady_clock::now(); // ˢ�³�ʼʱ��
}



