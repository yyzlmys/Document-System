#pragma once

#include <QObject>
#include <QTcpSocket>

class SignIn  : public QObject
{
	Q_OBJECT

public:
    explicit SignIn(QObject* parent = nullptr);

public slots:
    void sendData(int messageType, const QByteArray& data);

signals:
    void isLog(bool b);
};
