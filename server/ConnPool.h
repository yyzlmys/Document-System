#pragma once
#include <mysql.h>
#include <iostream>
#include <queue>
#include <string>
#include "MysqlConn.h"
#include <mutex>
#include <memory>
#include <condition_variable>
using namespace std;

class ConnPool
{
public:
    static ConnPool* getInstance();
    shared_ptr<MysqlConn> get();
    ~ConnPool();
private:
    static ConnPool* CP;
    ConnPool();
    ConnPool(const ConnPool& CP) = delete;
    ConnPool operator=(const ConnPool& CP) = delete;
    string ip, user, passwd, db;
    unsigned int port;
    int min, max, timeout, idletime;
    queue<MysqlConn*> connq;
    mutex mtx;
    condition_variable cv;
    void produce();
    void recycle();
    void addConn();
};

