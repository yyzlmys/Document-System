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
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);  // Set to network byte order
    stream << messageType;
    stream << static_cast<int>(data.size());
    stream.writeRawData(data.data(), data.size());

    QTcpSocket* skt = new QTcpSocket;
    connect(skt, &QTcpSocket::connected, this, [=]()
    {
        skt->write(message);
    });
    auto ip = QStringLiteral("123.60.157.65");
    auto port = 20245;
    skt->connectToHost(ip, port);
    
    connect(skt, &QTcpSocket::readyRead, this, [=]() //读服务器发来的布尔值
    {
        QByteArray data = skt->read(1);
        QString s(data.data());
        bool islog = s.toInt();
        emit isLog(islog);
        skt->close();
        skt->deleteLater();
    });
}

