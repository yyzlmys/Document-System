#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QJsonArray>

class Insert  : public QObject
{
	Q_OBJECT

public:
    explicit Insert(QObject* parent = nullptr);

public slots:
    void sendData(int messageType, const QByteArray& data);

signals:
    void insert_finish(int size, QJsonArray* res);

private:
    bool rh = false;
    int size = 0;
    QByteArray d;

};
