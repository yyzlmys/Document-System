#pragma once

#include <QWidget>
#include "ui_MainWindow5.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow5Class; };
QT_END_NAMESPACE

class MainWindow5 : public QWidget
{
	Q_OBJECT

public:
	MainWindow5(QWidget *parent = nullptr);
	~MainWindow5();

private:
	Ui::MainWindow5Class *ui;
};
