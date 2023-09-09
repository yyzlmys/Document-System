#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "MainWindow2.h"
#include <QCompleter>
#include <QTimer>
#include <QMessageBox>
#include <unordered_map>
#include <QJsonArray>
#include "sceneloader.h"
using namespace std;
#define sign_in 1
#define precise 2
#define range 3
#define query_all 4
#define update_one 5
#define insert1 6
#define query_key 7
#define insert2 8
#define insert3 9

MainWindow2::MainWindow2(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::MainWindow2Class())
{
	ui->setupUi(this);

    um_init();
    QStringList headers
    { 
        "��Ʊ��","������","����ʱ��","��������","��Ʒ����","����","���ڽ��","����","������ҽ��","���˷�","���ӷ�","FOB���",
        "��������","������", "��������","���ص���","�ᵥ��","����","����","��װ���","ë��","����","�����","��װ����",
        "�ջ�����һ","�ջ���һ","����һ","�������һ","�ջ����ڶ�","�ջ����","���ʶ�","������Ҷ�",
        "�ջ�������","�ջ�����","������","���������","�ջ�������","�ջ�����","������","���������",
        "�ջ�������","�ջ�����","������","���������","δ�ջ���","��������һ","�տ���һ","������һ","�������ڶ�","�տ��˶�","�������",
        "����������","�տ�����","��������","����������","�տ�����","��������","����������","�տ�����","��������",
        "����������","�տ�����","��������","��Ʊ����","��Ʊ���","δ�����","��ע" 
    };
    ui->table->setColumnCount(headers.size());
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table->setStyleSheet("QTableWidget::item { padding-left: 10px; padding-right: 10px; }");
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table->setHorizontalHeaderLabels(headers);
    ui->queryButton->setDisabled(true);
    ui->queryField->setEditable(true);
    ui->updateField->setEditable(true);
    ui->queryMethod->addItem("��ȷ��ѯ");
    ui->queryMethod->addItem("��Χ��ѯ");
    ui->queryMax->hide();
    ui->queryMin->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_9->hide();
    connect(ui->queryMethod, &QComboBox::currentTextChanged, [=]()
    {
        if (ui->queryMethod->currentText() == "��ȷ��ѯ")
        {
            ui->queryMax->hide();
            ui->queryMin->hide();
            ui->label_6->hide();
            ui->label_7->hide();
            ui->label_9->hide();
        }
        else if (ui->queryMethod->currentText() == "��Χ��ѯ")
        {
            ui->label_9->show();
            ui->queryMax->show();
            ui->queryMin->show();
            ui->label_6->show();
            ui->label_7->show();
        }
    });
    ui->queryField->addItems(headers);
    QCompleter* queryFieldCompleter = new QCompleter(headers, this);
    ui->queryField->setCompleter(queryFieldCompleter);
    ui->updateField->addItems(headers);
    QCompleter* updateFieldCompleter = new QCompleter(headers, this);
    ui->updateField->setCompleter(updateFieldCompleter);
    connect(ui->insertButton1,&QPushButton::clicked,this,[=]()
    {
        auto loader = SceneLoader::getInstance();
        loader->show(1);
    });
    connect(ui->insertButton2,&QPushButton::clicked,this,[=]()
    {
        auto loader = SceneLoader::getInstance();
        loader->show(2);
    });
    connect(ui->insertButton3,&QPushButton::clicked,this,[=]()
    {
        auto loader = SceneLoader::getInstance();
        loader->show(3);
    });

    qyt = new Query;
    queryThread = new QThread;
    queryThread->start();
    qyt->moveToThread(queryThread);

    connect(this, &MainWindow2::timetosend, qyt, &Query::sendData);
    QTimer* queryTimer = new QTimer(this);
    queryTimer->setSingleShot(true);
    QTimer* queryAllTimer = new QTimer(this);
    queryAllTimer->setSingleShot(true);
    QTimer* updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    connect(queryTimer, &QTimer::timeout, this, [=]()
    {
        ui->querySubmit->setEnabled(true);
    });
    connect(queryAllTimer, &QTimer::timeout, this, [=]()
    {
        ui->queryAll->setEnabled(true);
    });
    connect(updateTimer, &QTimer::timeout, this, [=]()
    {
        ui->updateSubmit->setEnabled(true);
    });
    connect(ui->querySubmit, &QPushButton::clicked, this, [=]()
    {
        ui->table->clearContents();
        ui->table->setRowCount(0);
        if (ui->querySubmit->isEnabled())
        {
            ui->querySubmit->setEnabled(false);
            queryTimer->start(2000);
        }
        if (ui->queryMethod->currentText() == "��ȷ��ѯ")
        {
            QString field = ui->queryField->currentText();
            QString content = ui->queryContent->text();
            QString s = um[field] + "^" + content;
            QByteArray msg = s.toUtf8();
            emit timetosend(precise, msg);
        }
        else if (ui->queryMethod->currentText() == "��Χ��ѯ")
        {
            QString field = ui->queryField->currentText();
            QString min = ui->queryMin->text();
            QString max = ui->queryMax->text();
            QString s = um[field] + "^" + min + "^" + max;
            QByteArray msg = s.toUtf8();
            emit timetosend(range, msg);      
        }
    });
    connect(qyt, &Query::query_finish, this, [=](int size, QJsonArray* res)
    {
        if (size == 0)
        {
            QMessageBox query_error;
            query_error.setIcon(QMessageBox::Critical);
            query_error.setWindowTitle("��ѯʧ�ܣ�");
            query_error.setText("û�д����¼�������������ݣ�");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
        }
        else if (size == -1)
        {
            QMessageBox query_error;
            query_error.setIcon(QMessageBox::Critical);
            query_error.setWindowTitle("�Ƿ���ѯ��");
            query_error.setText("�����ѯ�����Ƿ���Ϲ淶��");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
        }
        else if (size == -2)
        {
            QMessageBox query_error;
            query_error.setIcon(QMessageBox::Critical);
            query_error.setWindowTitle("�Ƿ����£�");
            query_error.setText("������������Ƿ���Ϲ淶��");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
        }
        else if (size == -3)
        {
            QString s = "";
            QByteArray msg = s.toUtf8();
            ui->table->clearContents();
            ui->table->setRowCount(0);
            emit timetosend(query_all, msg);
            QMessageBox query_error;
            query_error.setIcon(QMessageBox::Information);
            query_error.setWindowTitle("��ϲ��");
            query_error.setText("���³ɹ���");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
        }
        else
        {
            for (int i = 0; i < res->size(); i++)
            {
                ui->table->insertRow(i);
                auto row = res->at(i).toArray();
                for (int j = 0; j < row.size(); j++)
                {
                    QTableWidgetItem* item = new QTableWidgetItem(row[j].toString());
                    item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
                    ui->table->setItem(i, j, item);
                }
            }
            delete res;
        }
    });
    connect(ui->queryAll, &QPushButton::clicked, this, [=]()
    {
        ui->table->clearContents();
        ui->table->setRowCount(0);
        if (ui->queryAll->isEnabled())
        {
            ui->queryAll->setEnabled(false);
            queryAllTimer->start(2000);
        }
        QString s = "";
        QByteArray msg = s.toUtf8();
        emit timetosend(query_all, msg);
    });
    connect(ui->updateSubmit, &QPushButton::clicked, this, [=]()
{
        if (ui->updateSubmit->isEnabled())
        {
            ui->updateSubmit->setEnabled(false);
            updateTimer->start(2000);
        }
        if (ui->table->rowCount() == 0)
        {
            QMessageBox query_error;
            query_error.setIcon(QMessageBox::Critical);
            query_error.setWindowTitle("ע�⣡");
            query_error.setText("��ǰ�޿������ݣ�");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
            return;
        }
        int index = ui->updateIndex->text().toInt();
        if (index > ui->table->rowCount() || index <= 0)
        {
            QMessageBox query_error;
            query_error.setIcon(QMessageBox::Critical);
            query_error.setWindowTitle("�Ƿ���ţ�");
            query_error.setText("����ָ��������Ƿ�Ϸ���");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
            return;
        }
        QString field = ui->updateField->currentText();
        QString content = ui->updateContent->text();
        QString key = ui->table->item(index - 1, 0)->text();
        QMessageBox messageBox;
        messageBox.setWindowTitle("ȷ�ϸ���");
        messageBox.setText("��ȷ��Ҫִ�����¸�����\n\n�ڵ�" + QString::number(index) + "����¼\n�ֶΣ�" + field + "\n�������ݣ�" + content);
        messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        messageBox.setIcon(QMessageBox::Question);
        int result = messageBox.exec();
        if (result == QMessageBox::Ok)
        {
            QString s = um[field] + "^" + content + "^" + key;
            QByteArray msg = s.toUtf8();
            emit timetosend(update_one, msg); 
        }
    });
    QString s = "";
    QByteArray msg = s.toUtf8();
    emit timetosend(query_all, msg);
}

MainWindow2::~MainWindow2()
{
    delete qyt;
    qyt = nullptr;
    queryThread->exit();
    queryThread->wait();
    delete queryThread;
    queryThread = nullptr;
	delete ui;
}

void MainWindow2::um_init()
{
    um["��Ʊ��"] = "invoice_num";
    um["������"] = "order_num";
    um["����ʱ��"] = "order_date";
    um["��������"] = "trader_name";
    um["��Ʒ����"] = "goods_name";
    um["����"] = "goods_quantity";
    um["���ڽ��"] = "export_amount";
    um["����"] = "ex_rate";
    um["������ҽ��"] = "rmb_amount";
    um["���˷�"] = "ocean_freight";
    um["���ӷ�"] = "port_surcharge";
    um["FOB���"] = "fob_amount";
    um["��������"] = "forwarder_name";
    um["������"] = "supplier_name";
    um["��������"] = "export_date";
    um["���ص���"] = "clearance_num";
    um["�ᵥ��"] = "lading_num";
    um["����"] = "ship_name";
    um["����"] = "flight_num";
    um["��װ���"] = "container_num";
    um["ë��"] = "gross_weight";
    um["����"] = "net_weight";
    um["�����"] = "total_volume";
    um["��װ����"] = "packages_num";
    um["�ջ�����һ"] = "collect_date1";
    um["�ջ���һ"] = "collect_amount1";
    um["����һ"] = "ex_rate1";
    um["�������һ"] = "rmb_amount1";
    um["�ջ����ڶ�"] = "collect_date2";
    um["�ջ����"] = "collect_amount2";
    um["���ʶ�"] = "ex_rate2";
    um["������Ҷ�"] = "rmb_amount2";
    um["�ջ�������"] = "collect_date3";
    um["�ջ�����"] = "collect_amount3";
    um["������"] = "ex_rate3";
    um["���������"] = "rmb_amount3";
    um["�ջ�������"] = "collect_date4";
    um["�ջ�����"] = "collect_amount4";
    um["������"] = "ex_rate4";
    um["���������"] = "rmb_amount4";
    um["�ջ�������"] = "collect_date5";
    um["�ջ�����"] = "collect_amount5";
    um["������"] = "ex_rate5";
    um["���������"] = "rmb_amount5";
    um["δ�ջ���"] = "uncollect_amount";
    um["��������һ"] = "pay_date1";
    um["�տ���һ"] = "gain_person1";
    um["������һ"] = "pay_amount1";
    um["�������ڶ�"] = "pay_date2";
    um["�տ��˶�"] = "gain_person2";
    um["�������"] = "pay_amount2";
    um["����������"] = "pay_date3";
    um["�տ�����"] = "gain_person3";
    um["��������"] = "pay_amount3";
    um["����������"] = "pay_date4";
    um["�տ�����"] = "gain_person4";
    um["��������"] = "pay_amount4";
    um["����������"] = "pay_date5";
    um["�տ�����"] = "gain_person5";
    um["��������"] = "pay_amount5";
    um["����������"] = "pay_date6";
    um["�տ�����"] = "gain_person6";
    um["��������"] = "pay_amount6";
    um["��Ʊ����"] = "invoice_date";
    um["��Ʊ���"] = "invoice_amount";
    um["δ�����"] = "uninvoice_amount";
    um["��ע"] = "remark";
}








