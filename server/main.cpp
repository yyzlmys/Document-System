#include <iostream>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <string.h>
#include <strings.h>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <json.h>
#include <mutex>
#include "ThreadPool.h"
#include "ConnPool.h"
#define sign_in 1
#define precise 2
#define range 3
#define query_all 4
#define update_one 5
#define insert1 6
#define query_key 7
#define insert2 8
#define insert3 9
using namespace std;
using namespace Json;

int epfd;
mutex mtx;
bool readn(int fd, void* buf, int cnt);
bool sendn(int fd, char* buf, int cnt);
void communicate(int fd);

struct Msg 
{
	int type;
	int length;
	char* data = nullptr;  
};

void communicate(int fd)
{
	Msg msg;
	if (readn(fd, &msg.type, sizeof(int)))
	{
		msg.type = ntohl(msg.type);
		cout << "type = " << msg.type << endl;
		if (!readn(fd, &msg.length, sizeof(int))) return;
		msg.length = ntohl(msg.length);
		cout << "len = " << msg.length << endl;
	}
	else return;
	cout << "begin scan type" << endl;

	switch (msg.type)
	{
	case sign_in: {
		cout << "sign_in" << endl;
		msg.data = new char[msg.length + 1];
		if (!readn(fd, msg.data, msg.length)) return;
		msg.data[msg.length] = '\0';
		string s(msg.data);
		delete[] msg.data;
		msg.data = nullptr;
		int pos = s.find('@');
		string account = s.substr(0, pos);
		string passwd = s.substr(pos + 1);
		cout << account << endl << passwd << endl;

		auto cpool = ConnPool::getInstance();
		auto conn = cpool->get();

		bool ret = conn->query("select * from users where account = '" + account + "' && passwd = '" + passwd + "'; ");
		cout << "query res: " << ret << endl;
		string crct_act, crct_psd;
		auto row = conn->next();
		char b[1];
		if (row) b[0] = '1';
		else b[0] = '0';
		cout << "sign in res: " << b[0] << endl;
		sendn(fd, b, 1);

		break; }

	case precise: {
		cout << "precise" << endl;
		msg.data = new char[msg.length + 1];
		if (!readn(fd, msg.data, msg.length)) return;
		msg.data[msg.length] = '\0';
		string s(msg.data);
		delete[] msg.data;
		msg.data = nullptr;
		int pos = s.find('^');
		string field = s.substr(0, pos);
		string content = s.substr(pos + 1);
		cout << field << endl << content << endl;

		auto cpool = ConnPool::getInstance();
		auto conn = cpool->get();

		bool query_res;
		if (field == "goods_quantity" || field == "export_amount" || field == "ex_rate" || field == "rmb_amount" || field == "ocean_freight" || field == "port_surcharge" || field == "fob_amount" || field == "gross_weight" || field == "net_weight" || field == "total_volume" || field == "packages_num" || field == "collect_amount1" || field == "collect_amount2" || field == "collect_amount3" || field == "collect_amount4" || field == "collect_amount5" || field == "ex_rate1" || field == "ex_rate2" || field == "ex_rate3" || field == "ex_rate4" || field == "ex_rate5" || field == "rmb_amount1" || field == "rmb_amount2" || field == "rmb_amount3" || field == "rmb_amount4" || field == "rmb_amount5" || field == "uncollect_amount" || field == "pay_amount1" || field == "pay_amount2" || field == "pay_amount3" || field == "pay_amount4" || field == "pay_amount5" || field == "pay_amount6" || field == "invoice_amount" || field == "uninvoice_amount")
		{
			query_res = conn->query("select * from main_table where " + field + " = " + content + " ; ");
		}
		else
		{
			query_res = conn->query("select * from main_table where " + field + " = '" + content + "' ; ");
		}
		cout << "query res: " << query_res << endl;
		if (!query_res)
		{
			int query_error = -1;
			char b[sizeof(int)];
			memcpy(b, &query_error, sizeof(int));
			sendn(fd, b, sizeof(int));
			cout << "query error !" << endl;
		}
		else
		{
			bool isEmpty = true;
			Value root;
			while (auto row = conn->next())
			{
				isEmpty = false;
				Value jrow;
				for (int i = 0; i < row->cnt; i++)
				{
					auto field_data = row->get(i);
					if (field_data) jrow.append(field_data);
					else jrow.append("");
				}
				root.append(jrow);
			}
			if (isEmpty)
			{
				int size = 0;
				char b[sizeof(int)];
				memcpy(b, &size, sizeof(int));
				sendn(fd, b, sizeof(int));
				cout << "no res" << endl;
			}
			else
			{
				FastWriter fw;
				string ss = fw.write(root);
				int size = ss.size();
				char* b = new char[sizeof(int) + size];
				memcpy(b, &size, sizeof(int));
				memcpy(b + sizeof(int), ss.c_str(), size);
				sendn(fd, b, sizeof(int) + size);
				usleep(5000);
				delete[] b;
			}
		}

		break; }

	case range: {
		cout << "range" << endl;
		msg.data = new char[msg.length + 1];
		if (!readn(fd, msg.data, msg.length)) return;
		msg.data[msg.length] = '\0';
		string s(msg.data);
		delete[] msg.data;
		msg.data = nullptr;
		string field, min, max;
		istringstream iss(s);
		getline(iss, field, '^');
		getline(iss, min, '^');
		getline(iss, max);
		cout << field << endl << min << endl << max << endl;

		auto cpool = ConnPool::getInstance();
		auto conn = cpool->get();

		bool query_res;
		query_res = conn->query("select * from main_table where " + field + " between " + min + " and " + max + " ; ");
		cout << "query res: " << query_res << endl;
		if (!query_res)
		{
			int size = -1;
			char b[sizeof(int)];
			memcpy(b, &size, sizeof(int));
			sendn(fd, b, sizeof(int));
			cout << "query error !" << endl;
		}
		else
		{
			bool isEmpty = true;
			Value root;
			while (auto row = conn->next())
			{
				isEmpty = false;
				Value jrow;
				for (int i = 0; i < row->cnt; i++)
				{
					auto field_data = row->get(i);
					if (field_data) jrow.append(field_data);
					else jrow.append("");
				}
				root.append(jrow);
			}
			if (isEmpty)
			{
				int size = 0;
				char b[sizeof(int)];
				memcpy(b, &size, sizeof(int));
				sendn(fd, b, sizeof(int));
				cout << "no res" << endl;
			}
			else
			{
				FastWriter fw;
				string ss = fw.write(root);
				int size = ss.size();
				char b[sizeof(int) + size];
				memcpy(b, &size, sizeof(int));
				memcpy(b + sizeof(int), ss.c_str(), size);
				sendn(fd, b, sizeof(int) + size);
			}
		}

		break; }

	case query_all: {
		cout << "query_all" << endl;

		auto cpool = ConnPool::getInstance();
		auto conn = cpool->get();

		bool query_res = conn->query("select * from main_table;");
		Value root;
		while (auto row = conn->next())
		{
			Value jrow;
			for (int i = 0; i < row->cnt; i++)
			{
				auto field_data = row->get(i);
				if (field_data) jrow.append(field_data);
				else jrow.append("");
			}
			root.append(jrow);
		}

		FastWriter fw;
		string ss = fw.write(root);
		int size = ss.size();
		cout << "sent size  is " << size << endl;
		char b[sizeof(int) + size];
		memcpy(b, &size, sizeof(int));
		memcpy(b + sizeof(int), ss.c_str(), size);
		sendn(fd, b, sizeof(int) + size);

		cout << "query all end" << endl;

		break; }

	case update_one: {
		cout << "update" << endl;
		msg.data = new char[msg.length + 1];
		if (!readn(fd, msg.data, msg.length)) return;
		msg.data[msg.length] = '\0';
		string s(msg.data);
		delete[] msg.data;
		msg.data = nullptr;
		string field, content, key;
		istringstream iss(s);
		getline(iss, field, '^');
		getline(iss, content, '^');
		getline(iss, key);
		cout << field << endl << content << endl << key << endl;

		auto cpool = ConnPool::getInstance();
		auto conn = cpool->get();

		bool update_res;
		if (field == "goods_quantity" || field == "export_amount" || field == "ex_rate" || field == "rmb_amount" || field == "ocean_freight" || field == "port_surcharge" || field == "fob_amount" || field == "gross_weight" || field == "net_weight" || field == "total_volume" || field == "packages_num" || field == "collect_amount1" || field == "collect_amount2" || field == "collect_amount3" || field == "collect_amount4" || field == "collect_amount5" || field == "ex_rate1" || field == "ex_rate2" || field == "ex_rate3" || field == "ex_rate4" || field == "ex_rate5" || field == "rmb_amount1" || field == "rmb_amount2" || field == "rmb_amount3" || field == "rmb_amount4" || field == "rmb_amount5" || field == "uncollect_amount" || field == "pay_amount1" || field == "pay_amount2" || field == "pay_amount3" || field == "pay_amount4" || field == "pay_amount5" || field == "pay_amount6" || field == "invoice_amount" || field == "uninvoice_amount")
		{
			update_res = conn->update("update main_table set " + field + " = " + content + " where invoice_num = '" + key + "' ;");
		}
		else
		{
			update_res = conn->update("update main_table set " + field + " = '" + content + "' where invoice_num = '" + key + "' ;");
		}
		cout << "update res: " << update_res << endl;
		if (!update_res)
		{
			int update_error = -2;
			char b[sizeof(int)];
			memcpy(b, &update_error, sizeof(int));
			sendn(fd, b, sizeof(int));
			cout << "update error !" << endl;
		}
		else
		{
			int update_success = -3;
			char b[sizeof(int)];
			memcpy(b, &update_success, sizeof(int));
			sendn(fd, b, sizeof(int));
			cout << "update success !" << endl;
		}

		break; }
	
	case insert1: {
		cout << "insert1" << endl;
		msg.data = new char[msg.length + 1];
		if (!readn(fd, msg.data, msg.length)) return;
		msg.data[msg.length] = '\0';
		string s(msg.data);
		delete[] msg.data;
		msg.data = nullptr;
		istringstream iss(s); // ���ַ�����װ��������
		string parts[14]; // �洢ÿ�����ֵ��ַ���
		string v = "";
		for (int i = 0; i < 14; ++i) 
		{
			std::getline(iss, parts[i], '^');
			if (i < 5 || i>11)
			{
				parts[i] = '\'' + parts[i] + '\'';
			}
			v = v + parts[i] + ",";
		}
		v.pop_back();

		auto cpool = ConnPool::getInstance();
		auto conn = cpool->get();

		string sql = "insert into main_table  (invoice_num, order_num, order_date, trader_name, goods_name, goods_quantity, export_amount, ex_rate, rmb_amount, ocean_freight, port_surcharge, fob_amount, forwarder_name, supplier_name) values (" + v + ");";
		cout << sql << endl;
		bool insert_res = conn->update(sql);
		cout << "insert1 res: " << insert_res << endl;
		if (!insert_res)
		{
			int insert_error = -1;
			char b[sizeof(int)];
			memcpy(b, &insert_error, sizeof(int));
			sendn(fd, b, sizeof(int));
			cout << "insert1 error !" << endl;
		}
		else
		{
			int insert_error = -2;
			char b[sizeof(int)];
			memcpy(b, &insert_error, sizeof(int));
			sendn(fd, b, sizeof(int));
			cout << "insert1 success !" << endl;
		}

		break; }

	case query_key: {
		cout << "query_key" << endl;
		auto cpool = ConnPool::getInstance();
		auto conn = cpool->get();
		bool query_res = conn->query("select invoice_num from main_table;");
		cout << "query res: " << query_res << endl;
		Value root;
		while (auto row = conn->next())
		{
			auto field_data = row->get(0);
			if (field_data) root.append(field_data);
		}
		FastWriter fw;
		string s = fw.write(root);
		int size = s.size();
		char b[sizeof(int) + size];
		memcpy(b, &size, sizeof(int));
		memcpy(b + sizeof(int), s.c_str(), size);
		sendn(fd, b, sizeof(int) + size);

		cout << "query key end" << endl;

		break; }

	case insert2: {
		cout << "insert2" << endl;
		msg.data = new char[msg.length + 1];
		if (!readn(fd, msg.data, msg.length)) return;
		msg.data[msg.length] = '\0';
		string s(msg.data);
		delete[] msg.data;
		msg.data = nullptr;
		istringstream iss(s); // ���ַ�����װ��������
		string parts[11]; // �洢ÿ�����ֵ��ַ���
		for (int i = 0; i < 11; ++i)
		{
			std::getline(iss, parts[i], '^');
			if (i <= 6)
			{
				parts[i] = '\'' + parts[i] + '\'';
			}
		}

		auto cpool = ConnPool::getInstance();
		auto conn = cpool->get();

		bool query_res = conn->query("select * from main_table where invoice_num = " + parts[0] + " ;");
		bool isEmpty = true;
		if (conn->next()) isEmpty = false;
		if (isEmpty)
		{
			int insert_null = -3;
			char b[sizeof(int)];
			memcpy(b, &insert_null, sizeof(int));
			sendn(fd, b, sizeof(int));
			cout << "insert2 null !" << endl;
			break;
		}

 		string sql = "update main_table set export_date = " + parts[1] + ", clearance_num = " + parts[2] +", lading_num = " + parts[3] + ", ship_name = " + parts[4] + ", flight_num = " + parts[5] + ", container_num = " + parts[6] + ", gross_weight = " + parts[7] + ", net_weight = " + parts[8] + ", total_volume  = " + parts[9] + ", packages_num = " + parts[10] + " where invoice_num = " + parts[0] + " ;";
		cout << sql << endl;
		bool insert_res = conn->update(sql);
		cout << "insert2 res: " << insert_res << endl;
		if (!insert_res)
		{
			int insert_error = -1;
			char b[sizeof(int)];
			memcpy(b, &insert_error, sizeof(int));
			sendn(fd, b, sizeof(int));
			cout << "insert2 error !" << endl;
		}
		else
		{
			int insert_error = -2;
			char b[sizeof(int)];
			memcpy(b, &insert_error, sizeof(int));
			sendn(fd, b, sizeof(int));
			cout << "insert2 success !" << endl;
		}

		break; }

	case insert3: {
		cout << "insert3" << endl;
		msg.data = new char[msg.length + 1];
		if (!readn(fd, msg.data, msg.length)) return;
		msg.data[msg.length] = '\0';
		string s(msg.data);
		delete[] msg.data;
		msg.data = nullptr;
		istringstream iss(s); // ���ַ�����װ��������
		string parts[44]; // �洢ÿ�����ֵ��ַ���
		for (int i = 0; i < 44; ++i)
		{
			std::getline(iss, parts[i], '^');
			if (i == 1 || i == 5 || i == 9 || i == 13 || i == 17 || i == 22 || i == 23 || i == 25 || i == 26 || i == 28 || i == 29 || i == 31 || i == 32 || i == 34 || i == 35 || i == 37 || i == 38 || i == 40 || i == 43 || i==0)
			{
				parts[i] = '\'' + parts[i] + '\'';
			}
		}

		auto cpool = ConnPool::getInstance();
		auto conn = cpool->get();

		bool query_res = conn->query("select * from main_table where invoice_num = " + parts[0] + " ;");
		bool isEmpty = true;
		if (conn->next()) isEmpty = false;
		if (isEmpty)
		{
			int insert_null = -3;
			char b[sizeof(int)];
			memcpy(b, &insert_null, sizeof(int));
			sendn(fd, b, sizeof(int));
			cout << "insert3 null !" << endl;
			break;
		}

		string sql = "update main_table set collect_date1 = " + parts[1] + ", collect_amount1 = " + parts[2] + ", ex_rate1 = " + parts[3] + ", rmb_amount1 = " + parts[4] + ", collect_date2 = " + parts[5] + ", collect_amount2 = " + parts[6] + ", ex_rate2 = " + parts[7] + ", rmb_amount2 = " + parts[8] + ", collect_date3 = " + parts[9] + ", collect_amount3 = " + parts[10] + ", ex_rate3 = " + parts[11] + ", rmb_amount3 = " + parts[12] + ", collect_date4 = " + parts[13] + ", collect_amount4 = " + parts[14] + ", ex_rate4 = " + parts[15] + ", rmb_amount4 = " + parts[16] + ", collect_date5 = " + parts[17] + ", collect_amount5 = " + parts[18] + ", ex_rate5 = " + parts[19] + ", rmb_amount5 = " + parts[20] + ", uncollect_amount = " + parts[21] + ", pay_date1 = " + parts[22] + ", gain_person1 = " + parts[23] + ", pay_amount1 = " + parts[24] + ", pay_date2 = " + parts[25] + ", gain_person2 = " + parts[26] + ", pay_amount2 = " + parts[27] + ", pay_date3 = " + parts[28] + ", gain_person3 = " + parts[29] + ", pay_amount3 = " + parts[30] + ", pay_date4 = " + parts[31] + ", gain_person4 = " + parts[32] + ", pay_amount4 = " + parts[33] + ", pay_date5 = " + parts[34] + ", gain_person5 = " + parts[35] + ", pay_amount5 = " + parts[36] + ", pay_date6 = " + parts[37] + ", gain_person6 = " + parts[38] + ", pay_amount6 = " + parts[39] + ", invoice_date = " + parts[40] + ", invoice_amount = " + parts[41] + ", uninvoice_amount = " + parts[42] + ", remark = " + parts[43] + " where invoice_num = " + parts[0] + " ;";
		cout << sql << endl;
		bool insert_res = conn->update(sql);
		cout << "insert3 res: " << insert_res << endl;
		if (!insert_res)
		{
			int insert_error = -1;
			char b[sizeof(int)];
			memcpy(b, &insert_error, sizeof(int));
			sendn(fd, b, sizeof(int));
			cout << "insert3 error !" << endl;
		}
		else
		{
			int insert_error = -2;
			char b[sizeof(int)];
			memcpy(b, &insert_error, sizeof(int));
			sendn(fd, b, sizeof(int));
			cout << "insert3 success !" << endl;
		}

		break; }
	}
	


	//int len = 0, ptr = 0;
	//char buf[4096];
	//char tmp[1024];
	//while (len = recv(fd, tmp, sizeof tmp, 0) > 0)
	//{
	//	memcpy(buf + ptr, tmp, len);
	//	ptr += len;
	//}
	//if (len == -1 && errno == EAGAIN) // ������ģʽ�¶�������
	//{
	//	char method[5];
	//	char path[1024];
	//	sscanf(buf, "%[^ ] %[^ ]", method, path);
	//	if (strcasecmp(method, "get") != 0) continue;

	//}
	//else if (len == 0)
	//{
	//	epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
	//	close(fd);
	//}
	//else
	//{
	//	perror("recv error");
	//}
}

int main()
{
	auto tpool = Threadpool::getInstance();
	tpool->init(8);

	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(20245);
	//addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "localhost", &(addr.sin_addr.s_addr));
	bind(lfd, (sockaddr*)&addr, sizeof addr);
	listen(lfd, 128);

	epfd = epoll_create(1);
	epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = lfd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
	epoll_event evs[1024];
	int size = sizeof(evs) / sizeof(evs[0]);
	while (1)
	{
		int num = epoll_wait(epfd, evs, size, -1);
		for (int i = 0; i < num; i++)
		{
			int fd = evs[i].data.fd;
			if (fd == lfd)
			{
				sockaddr_in caddr;
				unsigned int caddr_size = sizeof(caddr);
				int cfd = accept(lfd, (sockaddr*)&caddr, &caddr_size);
				if (cfd == -1)
				{
					perror("connect failed ! ");
					continue;
				}
				char ip[32];
				cout << "client ip:" << inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, sizeof(ip))
					<< "  port:" << ntohs(caddr.sin_port) << endl;

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = cfd;
				unique_lock<std::mutex> lock(mtx);
				epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
				lock.unlock();
			}
			else
			{
				tpool->enqueue(communicate, fd);
				cout << "new event, fd: " << fd << endl;
			}
		}
	}
	
	epoll_ctl(epfd, EPOLL_CTL_DEL, lfd, NULL);
	close(lfd);
	close(epfd);
	return 0;
}

bool readn(int fd, void* buf, int cnt)
{
	int left = cnt;
	int nread = 0;
	char* tp = static_cast<char*>(buf);
	while (left > 0)
	{
		nread = read(fd, tp, left);
		if (nread == -1)
		{
			perror("readn error");
			return false;
		}
		else if (nread == 0)
		{
			cout << "client disconnect ! close fd " << fd << endl;
			unique_lock<std::mutex> lock(mtx);
			epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
			lock.unlock();
			close(fd);
			return false;
		}
		tp += nread;
		left -= nread;
	}
	return true;
}

bool sendn(int fd, char* buf, int cnt)
{
	int left = cnt;
	int nwrite = 0;
	char* tp = buf;
	while (left > 0)
	{
		nwrite = write(fd, tp, left);
		if (nwrite == -1)
		{
			perror("writen error");
			return false;
		}
		else if (nwrite == 0)
		{
			continue;
		}
		tp += nwrite;
		left -= nwrite;
	}
	return true;
}







