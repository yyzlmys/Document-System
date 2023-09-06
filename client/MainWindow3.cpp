#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "MainWindow3.h"
#include <QCompleter>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <QJsonArray>
#include "sceneloader.h"
#define sign_in 1
#define precise 2
#define range 3
#define query_all 4
#define update_one 5
#define insert1 6
#define query_key 7
#define insert2 8
#define insert3 9

MainWindow3::MainWindow3(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::MainWindow3Class())
{
	ui->setupUi(this);

    ui->submit_2->hide();
    ui->insertButton1->setDisabled(true);
    ui->trader_name->setEditable(true);
    ui->goods_name->setEditable(true);
    ui->forwarder_name->setEditable(true);
    ui->supplier_name->setEditable(true);
    QStringList* trader_names = new QStringList;
    QStringList* goods_names = new QStringList;
    QStringList* forwarder_names = new QStringList;
    QStringList* supplier_names = new QStringList;
    connect(ui->queryButton, &QPushButton::clicked, this, [=]()
    {
        auto loader = SceneLoader::getInstance();
        loader->show(0);
    });
    connect(ui->insertButton2, &QPushButton::clicked, this, [=]()
    {
        auto loader = SceneLoader::getInstance();
        loader->show(2);
    });
    connect(ui->insertButton3, &QPushButton::clicked, this, [=]()
    {
        auto loader = SceneLoader::getInstance();
        loader->show(3);
    });

    it = new Insert;
    insertThread = new QThread;
    it->moveToThread(insertThread);
    insertThread->start();

    connect(this, &MainWindow3::timetosend, it, &Insert::sendData);
    connect(it, &Insert::insert_finish, this, [=](int size, QJsonArray* res)
    {
        if (size >= 0)
        {
            QString tn = res->at(3).toString();
            QString gn = res->at(4).toString();
            QString fn = res->at(12).toString();
            QString sn = res->at(13).toString();
            //trader_names->append(tn);
            //goods_names->append(gn);
            //forwarder_names->append(fn);
            //supplier_names->append(sn);
            delete res;
            ui->trader_name->addItem(tn);
            //QCompleter* trader_nameCompleter = new QCompleter(*trader_names, this);
            //ui->trader_name->setCompleter(trader_nameCompleter);
            ui->goods_name->addItem(gn);
            //QCompleter* goods_nameCompleter = new QCompleter(*goods_names, this);
            //ui->goods_name->setCompleter(goods_nameCompleter);
            ui->forwarder_name->addItem(fn);
            //QCompleter* forwarder_nameCompleter = new QCompleter(*forwarder_names, this);
            //ui->forwarder_name->setCompleter(forwarder_nameCompleter);
            ui->supplier_name->addItem(sn);
            //QCompleter* supplier_nameCompleter = new QCompleter(*supplier_names, this);
            //ui->supplier_name->setCompleter(supplier_nameCompleter);
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
    });
    connect(ui->submit, &QPushButton::clicked, this, [=]()
    {
        QString invoice_num = ui->invoice_num->text();
        QString order_num = ui->order_num->text();
        QString order_date = ui->order_date->text();
        QString trader_name = ui->trader_name->currentText();
        QString goods_name = ui->goods_name->currentText();
        QString goods_quantity = ui->goods_quantity->text();
        QString export_amount = ui->export_amount->text();
        QString ex_rate = ui->ex_rate->text();
        QString rmb_amount = ui->rmb_amount->text();
        QString ocean_freight = ui->ocean_freight->text();
        QString port_surcharge = ui->port_surcharge->text();
        QString fob_amount = ui->fob_amount->text();
        QString forwarder_name = ui->forwarder_name->currentText();
        QString supplier_name = ui->supplier_name->currentText();
        QString s = invoice_num + "^" + order_num + "^" + order_date + "^" + trader_name + "^" + goods_name + "^" +
            goods_quantity + "^" + export_amount + "^" + ex_rate + "^" + rmb_amount + "^" + ocean_freight + "^" +
            port_surcharge + "^" + fob_amount + "^" + forwarder_name + "^" + supplier_name;
        QByteArray msg = s.toUtf8();
        emit timetosend(insert1, msg);
    });
    QString s="";
    QByteArray msg=s.toUtf8();
    emit timetosend(query_all,msg);
}

MainWindow3::~MainWindow3()
{
    delete it;
    it = nullptr;
    insertThread->exit();
    insertThread->wait();
    delete insertThread;
    insertThread = nullptr;
	delete ui;
}
