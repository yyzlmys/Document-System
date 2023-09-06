#include "Query.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QThread>

Query::Query(QObject *parent)
	: QObject(parent)
{}

void Query::sendData(int messageType, const QByteArray& data)
{
    qDebug() << "query Send : " << QThread::currentThread();

    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);  // Set to network byte order
    stream << messageType;
    stream << static_cast<int>(data.size());
    stream.writeRawData(data.data(), data.size());

    QTcpSocket* clsocket = new QTcpSocket;
    auto ip = QStringLiteral("123.60.157.65");
    auto port = 20245;
    clsocket->connectToHost(ip, port);
    clsocket->write(message);
    connect(clsocket, &QTcpSocket::readyRead, this, [=]()
    {
        qDebug() << "query Recv : " << QThread::currentThread();

        QByteArray d = clsocket->read(sizeof(int));
        int size = *reinterpret_cast<const int*>(d.data());
        qDebug() << "the read size is " << size;
        d.clear();
        
        if (size > 0)
        {
            while (d.size() < size && clsocket->bytesAvailable())
            {
                d += clsocket->read(size - d.size());
            }
            QJsonArray* res = new QJsonArray;
            *res = QJsonDocument::fromJson(d).array();
            emit query_finish(size, res);
        }
        else if(size == -5)
        {
            clsocket->close();
            clsocket->deleteLater();  
        }
        else
        {
            emit query_finish(size, nullptr);
        }
        
    });
}
