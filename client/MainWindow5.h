#pragma once

#include "ui_MainWindow5.h"
#include <QWidget>
#include <QThread>
#include <QTcpSocket>
#include "Insert.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow5Class; };
QT_END_NAMESPACE

class MainWindow5 : public QWidget
{
	Q_OBJECT

public:
	explicit MainWindow5(QWidget *parent = nullptr);
	~MainWindow5();

private:
	Ui::MainWindow5Class *ui;
	Insert* it = nullptr;
	QThread* insertThread = nullptr;

signals:
	void timetosend(int messageType, const QByteArray& data);
};
