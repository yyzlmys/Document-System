#pragma once

#include <QtWidgets/QWidget>
#include "ui_MainWindow1.h"
#include <QTcpSocket>
#include "SignIn.h"

class MainWindow1 : public QWidget
{
    Q_OBJECT

public:
    MainWindow1(QWidget *parent = nullptr);
    ~MainWindow1();

private:
    Ui::MainWindow1Class ui;
    SignIn* st = nullptr;
    QThread* signThread = nullptr;

signals:
    void timetosend(int messageType, const QByteArray& data);
    void start();
    void delete_me(MainWindow1* mw1);
};
