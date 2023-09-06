#include "signin.h"
#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QtEndian>
#include <QThread>

SignIn::SignIn(QObject* parent)
    : QObject{ parent }
{

}

void SignIn::sendData(int messageType, const QByteArray& data)
{
    qDebug() << "sign Send : " << QThread::currentThread();

    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);  // Set to network byte order
    stream << messageType;
    stream << static_cast<int>(data.size());
    stream.writeRawData(data.data(), data.size());

    QTcpSocket* clsocket = new QTcpSocket;
    auto ip = "123.60.157.65";
    auto port = 20245;
    clsocket->connectToHost(QHostAddress(ip), port);
    clsocket->write(message);
    connect(clsocket, &QTcpSocket::readyRead, this, [=]() //读服务器发来的布尔值
    {
        qDebug() << "sign Recv : " << QThread::currentThread();

        QByteArray data = clsocket->read(1);
        QString s(data.data());
        bool islog = s.toInt();
        emit isLog(islog);
        clsocket->close();
        clsocket->deleteLater();
    });
}

