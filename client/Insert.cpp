#include "Insert.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QThread>

Insert::Insert(QObject *parent)
	: QObject(parent)
{}

void Insert::sendData(int messageType, const QByteArray& data)
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

    connect(skt, &QTcpSocket::readyRead, this, [=]()
    {
        if (!rh)
        {
            QByteArray dt;
            dt = skt->read(sizeof(int));
            size = *reinterpret_cast<const int*>(dt.data());
            rh = true;
        }

        if (size >= 0)
        {
            if (d.size() < size)
            {
                d.append(skt->readAll());
            }
            if (d.size() < size) return;
            QJsonArray* res = new QJsonArray;
            *res = QJsonDocument::fromJson(d).array();
            emit insert_finish(size, res);
        }
        else
        {
            emit insert_finish(size, nullptr);
        }
        skt->close();
        skt->deleteLater();
        rh = false;
        d.clear();
    });
}
