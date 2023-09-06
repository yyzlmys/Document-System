#pragma once
#include <string>
#include <mysql.h>
#include <chrono>
#include <iostream>
#include <memory>
using namespace std;
using namespace chrono;

struct Row
{
    MYSQL_ROW data;
    unsigned int cnt;
    Row() :data(nullptr), cnt(0) {}
    char* get(int i)
    {
        if (i >= cnt) return nullptr;
        return data[i];
    }
    char* operator[](int i)
    {
        if (i >= cnt) return nullptr;
        return data[i];
    }
};

class MysqlConn
{
private:
    MYSQL* conn = nullptr;
    MYSQL_RES* res = nullptr;
    steady_clock::time_point active;
public:
    MysqlConn();
    ~MysqlConn();
    bool connect(string ip, unsigned short port, string user, string passwd, string db);
    bool update(string sql);
    bool query(string sql);
    shared_ptr<Row> next();
    template<typename... Args>
    bool transaction(string sql, Args... args);
    bool transaction();
    long long getAliveTime();
    void refreshTime();
};
