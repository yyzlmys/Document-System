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
        "发票号","订单号","订单时间","外商名称","商品名称","数量","出口金额","汇率","折人民币金额","海运费","港杂费","FOB金额",
        "货代名称","供货商", "出口日期","报关单号","提单号","船名","航次","集装箱号","毛重","净重","总体积","包装件数",
        "收汇日期一","收汇金额一","汇率一","折人民币一","收汇日期二","收汇金额二","汇率二","折人民币二",
        "收汇日期三","收汇金额三","汇率三","折人民币三","收汇日期四","收汇金额四","汇率四","折人民币四",
        "收汇日期五","收汇金额五","汇率五","折人民币五","未收汇金额","付款日期一","收款人一","付款金额一","付款日期二","收款人二","付款金额二",
        "付款日期三","收款人三","付款金额三","付款日期四","收款人四","付款金额四","付款日期五","收款人五","付款金额五",
        "付款日期六","收款人六","付款金额六","开票日期","开票金额","未付金额","备注" 
    };
    ui->table->setColumnCount(headers.size());
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->table->setStyleSheet("QTableWidget::item { padding-left: 10px; padding-right: 10px; }");
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table->setHorizontalHeaderLabels(headers);
    ui->queryButton->setDisabled(true);
    ui->queryField->setEditable(true);
    ui->updateField->setEditable(true);
    ui->queryMethod->addItem("精确查询");
    ui->queryMethod->addItem("范围查询");
    ui->queryMax->hide();
    ui->queryMin->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_9->hide();
    connect(ui->queryMethod, &QComboBox::currentTextChanged, [=]()
    {
        if (ui->queryMethod->currentText() == "精确查询")
        {
            ui->queryMax->hide();
            ui->queryMin->hide();
            ui->label_6->hide();
            ui->label_7->hide();
            ui->label_9->hide();
        }
        else if (ui->queryMethod->currentText() == "范围查询")
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
        if (ui->queryMethod->currentText() == "精确查询")
        {
            QString field = ui->queryField->currentText();
            QString content = ui->queryContent->text();
            QString s = um[field] + "^" + content;
            QByteArray msg = s.toUtf8();
            emit timetosend(precise, msg);
        }
        else if (ui->queryMethod->currentText() == "范围查询")
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
            query_error.setWindowTitle("查询失败！");
            query_error.setText("没有此项记录，请检查输入内容！");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
        }
        else if (size == -1)
        {
            QMessageBox query_error;
            query_error.setIcon(QMessageBox::Critical);
            query_error.setWindowTitle("非法查询！");
            query_error.setText("请检查查询内容是否符合规范！");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
        }
        else if (size == -2)
        {
            QMessageBox query_error;
            query_error.setIcon(QMessageBox::Critical);
            query_error.setWindowTitle("非法更新！");
            query_error.setText("请检查更新内容是否符合规范！");
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
            query_error.setWindowTitle("恭喜！");
            query_error.setText("更新成功！");
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
            query_error.setWindowTitle("注意！");
            query_error.setText("当前无可用数据！");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
            return;
        }
        int index = ui->updateIndex->text().toInt();
        if (index > ui->table->rowCount() || index <= 0)
        {
            QMessageBox query_error;
            query_error.setIcon(QMessageBox::Critical);
            query_error.setWindowTitle("非法序号！");
            query_error.setText("请检查指定的序号是否合法！");
            query_error.setStandardButtons(QMessageBox::Ok);
            query_error.exec();
            return;
        }
        QString field = ui->updateField->currentText();
        QString content = ui->updateContent->text();
        QString key = ui->table->item(index - 1, 0)->text();
        QMessageBox messageBox;
        messageBox.setWindowTitle("确认更新");
        messageBox.setText("你确定要执行如下更新吗？\n\n在第" + QString::number(index) + "条记录\n字段：" + field + "\n更新内容：" + content);
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
    um["发票号"] = "invoice_num";
    um["订单号"] = "order_num";
    um["订单时间"] = "order_date";
    um["外商名称"] = "trader_name";
    um["商品名称"] = "goods_name";
    um["数量"] = "goods_quantity";
    um["出口金额"] = "export_amount";
    um["汇率"] = "ex_rate";
    um["折人民币金额"] = "rmb_amount";
    um["海运费"] = "ocean_freight";
    um["港杂费"] = "port_surcharge";
    um["FOB金额"] = "fob_amount";
    um["货代名称"] = "forwarder_name";
    um["供货商"] = "supplier_name";
    um["出口日期"] = "export_date";
    um["报关单号"] = "clearance_num";
    um["提单号"] = "lading_num";
    um["船名"] = "ship_name";
    um["航次"] = "flight_num";
    um["集装箱号"] = "container_num";
    um["毛重"] = "gross_weight";
    um["净重"] = "net_weight";
    um["总体积"] = "total_volume";
    um["包装件数"] = "packages_num";
    um["收汇日期一"] = "collect_date1";
    um["收汇金额一"] = "collect_amount1";
    um["汇率一"] = "ex_rate1";
    um["折人民币一"] = "rmb_amount1";
    um["收汇日期二"] = "collect_date2";
    um["收汇金额二"] = "collect_amount2";
    um["汇率二"] = "ex_rate2";
    um["折人民币二"] = "rmb_amount2";
    um["收汇日期三"] = "collect_date3";
    um["收汇金额三"] = "collect_amount3";
    um["汇率三"] = "ex_rate3";
    um["折人民币三"] = "rmb_amount3";
    um["收汇日期四"] = "collect_date4";
    um["收汇金额四"] = "collect_amount4";
    um["汇率四"] = "ex_rate4";
    um["折人民币四"] = "rmb_amount4";
    um["收汇日期五"] = "collect_date5";
    um["收汇金额五"] = "collect_amount5";
    um["汇率五"] = "ex_rate5";
    um["折人民币五"] = "rmb_amount5";
    um["未收汇金额"] = "uncollect_amount";
    um["付款日期一"] = "pay_date1";
    um["收款人一"] = "gain_person1";
    um["付款金额一"] = "pay_amount1";
    um["付款日期二"] = "pay_date2";
    um["收款人二"] = "gain_person2";
    um["付款金额二"] = "pay_amount2";
    um["付款日期三"] = "pay_date3";
    um["收款人三"] = "gain_person3";
    um["付款金额三"] = "pay_amount3";
    um["付款日期四"] = "pay_date4";
    um["收款人四"] = "gain_person4";
    um["付款金额四"] = "pay_amount4";
    um["付款日期五"] = "pay_date5";
    um["收款人五"] = "gain_person5";
    um["付款金额五"] = "pay_amount5";
    um["付款日期六"] = "pay_date6";
    um["收款人六"] = "gain_person6";
    um["付款金额六"] = "pay_amount6";
    um["开票日期"] = "invoice_date";
    um["开票金额"] = "invoice_amount";
    um["未付金额"] = "uninvoice_amount";
    um["备注"] = "remark";
}








