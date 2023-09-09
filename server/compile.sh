g++ -g -std=c++20 -pthread -I/usr/include/mysql/ -I/usr/local/include/json/ main.cpp ConnPool.cpp MysqlConn.cpp ThreadPool.h -o huanuo.o -L/usr/lib64/mysql/ -lmysqlclient -L/usr/local/lib/ -ljsoncpp

