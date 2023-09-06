#pragma once

#include "ui_MainWindow2.h"
#include <QWidget>
#include <QThread>
#include <unordered_map>
#include <QTcpSocket>
#include "Query.h"
using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow2Class; };
QT_END_NAMESPACE

class MainWindow2 : public QWidget
{
	Q_OBJECT

private:
	Ui::MainWindow2Class *ui;
    unordered_map<QString, QString> um;
    void um_init();
    Query* qyt = nullptr;
    QThread* queryThread = nullptr;

public:
    explicit MainWindow2(QWidget* parent = nullptr);
    ~MainWindow2();

signals:
    void timetosend(int messageType, const QByteArray& data);
};
