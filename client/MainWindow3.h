#pragma once

#include <QWidget>
#include "ui_MainWindow3.h"
#include <QThread>
#include <QTcpSocket>
#include "Insert.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow3Class; };
QT_END_NAMESPACE

class MainWindow3 : public QWidget
{
	Q_OBJECT

public:
    explicit MainWindow3(QWidget* parent = nullptr);
    ~MainWindow3();

private:
    Ui::MainWindow3Class* ui;
    Insert* it = nullptr;
    QThread* insertThread = nullptr;

signals:
    void timetosend(int messageType, const QByteArray& data);
	
};
