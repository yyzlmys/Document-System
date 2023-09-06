#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "MainWindow4.h"
#include <QCompleter>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <QJsonArray>
#include "SceneLoader.h"
#define sign_in 1
#define precise 2
#define range 3
#define query_all 4
#define update_one 5
#define insert1 6
#define query_key 7
#define insert2 8
#define insert3 9

MainWindow4::MainWindow4(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    ui.insertButton2->setDisabled(true);
    ui.submit_2->hide();
    ui.invoice_num->setEditable(true);
    QStringList* invoice_nums = new QStringList;
    connect(ui.queryButton, &QPushButton::clicked, this, [=]()
    {
        auto loader = SceneLoader::getInstance();
        loader->show(0);
    });
    connect(ui.insertButton1, &QPushButton::clicked, this, [=]()
    {
        auto loader = SceneLoader::getInstance();
        loader->show(1);
    });
    connect(ui.insertButton3, &QPushButton::clicked, this, [=]()
    {
        auto loader = SceneLoader::getInstance();
        loader->show(3);
    });

    it = new Insert;
    insertThread = new QThread;
    it->moveToThread(insertThread);
    insertThread->start();

    connect(this, &MainWindow4::timetosend, it, &Insert::sendData);
    connect(it, &Insert::insert_finish, this, [=](int size, QJsonArray* res)
    {
        if (size >= 0)
        {
            for (int i = 0; i < res->size(); i++)
            {
                QString in = res->at(i).toString();
                invoice_nums->append(in);
            }
            delete res;
            ui.invoice_num->addItems(*invoice_nums);
            QCompleter* invoice_numCompleter = new QCompleter(*invoice_nums, this);
            ui.invoice_num->setCompleter(invoice_numCompleter);
        }
        else if (size == -1)
        {
            QMessageBox query_error;
            query_error.setIcon(QMessageBox::Critical);
            query_error.setWindowTitle("插入失败！");
            query_error.setText("请检查插入内容是否符合规范！");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
        }
        else if (size == -2)
        {
            QMessageBox query_error;
            query_error.setIcon(QMessageBox::Information);
            query_error.setWindowTitle("恭喜！");
            query_error.setText("插入成功！");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
        }
        else if (size == -3)
        {
            QMessageBox query_error;
            query_error.setIcon(QMessageBox::Critical);
            query_error.setWindowTitle("注意！");
            query_error.setText("未查询到该发票号，请再次检查！");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
        }
    });
    connect(ui.submit, &QPushButton::clicked, this, [=]()
    {
        QString invoice_num = ui.invoice_num->currentText();
        QString export_date = ui.export_date->text();
        QString clearance_num = ui.clearance_num->text();
        QString lading_num = ui.lading_num->text();
        QString ship_name = ui.ship_name->text();
        QString flight_num = ui.flight_num->text();
        QString container_num = ui.container_num->text();
        QString gross_weight = ui.gross_weight->text();
        QString net_weight = ui.net_weight->text();
        QString total_volume = ui.total_volume->text();
        QString packages_num = ui.packages_num->text();
        QString s = invoice_num + "^" + export_date + "^" + clearance_num + "^" + lading_num + "^" + ship_name + "^" +
            flight_num + "^" + container_num + "^" + gross_weight + "^" + net_weight + "^" + total_volume + "^" +
            packages_num;
        QByteArray msg = s.toUtf8();
        emit timetosend(insert2, msg);
    });
    QString s = "";
    QByteArray msg = s.toUtf8();
    emit timetosend(query_key, msg);
}

MainWindow4::~MainWindow4()
{
    delete it;
    it = nullptr;
    insertThread->exit();
    insertThread->wait();
    delete insertThread;
    insertThread = nullptr;
}
