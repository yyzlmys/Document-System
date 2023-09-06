#pragma once

#include "ui_MainWindow4.h"
#include <QWidget>
#include <QThread>
#include <QTcpSocket>
#include "Insert.h"

class MainWindow4 : public QWidget
{
	Q_OBJECT

public:
    explicit MainWindow4(QWidget* parent = nullptr);
    ~MainWindow4();

private:
    Ui::MainWindow4Class ui;
    Insert* it = nullptr;
    QThread* insertThread = nullptr;

signals:
    void timetosend(int messageType, const QByteArray& data);
};
