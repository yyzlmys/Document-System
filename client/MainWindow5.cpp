#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "MainWindow5.h"
#include <QCompleter>
#include <QTimer>
#include <QMessageBox>
#include <QJsonArray>
#include <QScrollArea>
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

MainWindow5::MainWindow5(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::MainWindow5Class())
{
	ui->setupUi(this);

    ui->label_51->setStyleSheet("QLabel { color : red; }");
    ui->insertButton3->setDisabled(true);
    ui->remark->setPlaceholderText("不超过30字");
    ui->invoice_num->setEditable(true);
    connect(ui->insertButton1, &QPushButton::clicked, this, [=]()
    {
        auto loader = SceneLoader::getInstance();
        loader->show(1);
    });
    connect(ui->insertButton2, &QPushButton::clicked, this, [=]()
    {
        auto loader = SceneLoader::getInstance();
        loader->show(2);
    });
    connect(ui->queryButton, &QPushButton::clicked, this, [=]()
    {
        auto loader = SceneLoader::getInstance();
        loader->show(0);
    });
    QStringList* invoice_nums = new QStringList;

    it = new Insert;
    insertThread = new QThread;
    insertThread->start();
    it->moveToThread(insertThread);

    connect(this, &MainWindow5::timetosend, it, &Insert::sendData);
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
            ui->invoice_num->addItems(*invoice_nums);
            QCompleter* invoice_numCompleter = new QCompleter(*invoice_nums, this);
            ui->invoice_num->setCompleter(invoice_numCompleter);
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
    connect(ui->submit, &QPushButton::clicked, this, [=]()
    {
        QString invoice_num = ui->invoice_num->currentText();
        QString collect_date1 = ui->collect_date1->text();
        QString collect_amount1 = ui->collect_amount1->text();
        QString ex_rate1 = ui->ex_rate1->text();
        QString rmb_amount1 = ui->rmb_amount1->text();
        QString collect_date2 = ui->collect_date2->text();
        QString collect_amount2 = ui->collect_amount2->text();
        QString ex_rate2 = ui->ex_rate2->text();
        QString rmb_amount2 = ui->rmb_amount2->text();
        QString collect_date3 = ui->collect_date3->text();
        QString collect_amount3 = ui->collect_amount3->text();
        QString ex_rate3 = ui->ex_rate3->text();
        QString rmb_amount3 = ui->rmb_amount3->text();
        QString collect_date4 = ui->collect_date4->text();
        QString collect_amount4 = ui->collect_amount4->text();
        QString ex_rate4 = ui->ex_rate4->text();
        QString rmb_amount4 = ui->rmb_amount4->text();
        QString collect_date5 = ui->collect_date5->text();
        QString collect_amount5 = ui->collect_amount5->text();
        QString ex_rate5 = ui->ex_rate5->text();
        QString rmb_amount5= ui->rmb_amount5->text();
        QString uncollect_amount = ui->uncollect_amount->text();
        QString pay_date1 = ui->pay_date1->text();
        QString gain_person1 = ui->gain_person1->text();
        QString pay_amount1 = ui->pay_amount1->text();
        QString pay_date2 = ui->pay_date2->text();
        QString gain_person2 = ui->gain_person2->text();
        QString pay_amount2 = ui->pay_amount2->text();
        QString pay_date3 = ui->pay_date3->text();
        QString gain_person3 = ui->gain_person3->text();
        QString pay_amount3 = ui->pay_amount3->text();
        QString pay_date4 = ui->pay_date4->text();
        QString gain_person4 = ui->gain_person4->text();
        QString pay_amount4= ui->pay_amount4->text();
        QString pay_date5 = ui->pay_date5->text();
        QString gain_person5 = ui->gain_person5->text();
        QString pay_amount5 = ui->pay_amount5->text();
        QString pay_date6 = ui->pay_date6->text();
        QString gain_person6 = ui->gain_person6->text();
        QString pay_amount6 = ui->pay_amount6->text();
        QString invoice_date = ui->invoice_date->text();
        QString invoice_amount = ui->invoice_amount->text();
        QString uninvoice_amount = ui->uninvoice_amount->text();
        QString remark = ui->remark->toPlainText();
        QString s = invoice_num + "^" + collect_date1 + "^" + collect_amount1 + "^" + ex_rate1 + "^" + rmb_amount1 + "^" + collect_date2 + "^" + collect_amount2 + "^" + ex_rate2 + "^" + rmb_amount2 + "^" + collect_date3 + "^" + collect_amount3 + "^" + ex_rate3 + "^" + rmb_amount3 + "^" + collect_date4 + "^" + collect_amount4 + "^" + ex_rate4 + "^" + rmb_amount4 + "^" + collect_date5 + "^" + collect_amount5 + "^" + ex_rate5 + "^" + rmb_amount5 + "^" + uncollect_amount + "^" + pay_date1 + "^" + gain_person1 + "^" + pay_amount1 + "^" + pay_date2 + "^" + gain_person2 + "^" + pay_amount2 + "^" + pay_date3 + "^" + gain_person3 + "^" + pay_amount3 + "^" + pay_date4 + "^" + gain_person4 + "^" + pay_amount4 + "^" + pay_date5 + "^" + gain_person5 + "^" + pay_amount5 + "^" + pay_date6 + "^" + gain_person6 + "^" + pay_amount6 + "^" + invoice_date + "^" + invoice_amount + "^" + uninvoice_amount + "^" + remark;
        QByteArray msg = s.toUtf8();
        emit timetosend(insert3, msg);
    });
    QString s = "";
    QByteArray msg = s.toUtf8();
    emit timetosend(query_key, msg);
}

MainWindow5::~MainWindow5()
{
	delete it;
	it = nullptr;
	insertThread->exit();
	insertThread->wait();
	delete insertThread;
	delete ui;
}








