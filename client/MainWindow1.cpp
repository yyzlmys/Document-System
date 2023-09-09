#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "sceneloader.h"
#include "MainWindow1.h"
#include <QTcpSocket>
#include <QThread>
#include <QMessageBox>
#define sign_in 1
#define precise 2
#define range 3
#define query_all 4
#define update_one 5
#define insert1 6
#define query_key 7
#define insert2 8
#define insert3 9

MainWindow1::MainWindow1(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.passwd->setEchoMode(QLineEdit::Password);

    st = new SignIn;
    signThread = new QThread;
    signThread->start();
    st->moveToThread(signThread);

    connect(this, &MainWindow1::timetosend, st, &SignIn::sendData);
    connect(ui.sign, &QPushButton::clicked, this, [=]() //µ„ª˜µ«¬º∞¥≈•
    {
        auto account = ui.account->text();
        auto passwd = ui.passwd->text();
        QString s = account + "@" + passwd;
        QByteArray msg = s.toUtf8();
        emit timetosend(sign_in, msg);
    });
    auto loader = SceneLoader::getInstance();
    connect(this, &MainWindow1::start, loader, &SceneLoader::init);
    connect(st, &SignIn::isLog, this, [=](bool b)
    {
        if (b)
        {
            auto loader = SceneLoader::getInstance();
            connect(this, &MainWindow1::delete_me, loader, &SceneLoader::delete1);
            delete st;
            st = nullptr;
            signThread->exit();
            signThread->wait();
            delete signThread;
            signThread = nullptr;
            loader->show(0);
            emit delete_me(this);
        }
        else
        {
            QMessageBox log_error;
            log_error.setIcon(QMessageBox::Critical);
            log_error.setWindowTitle("µ«¬º ß∞‹£°");
            log_error.setText("’À∫≈ªÚ√‹¬Î¥ÌŒÛ£°");
            log_error.setStandardButtons(QMessageBox::Ok);
            log_error.exec();
        }
    });

    emit start();
}

MainWindow1::~MainWindow1()
{}
