#include "MainWindow5.h"

MainWindow5::MainWindow5(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::MainWindow5Class())
{
	ui->setupUi(this);
}

MainWindow5::~MainWindow5()
{
	delete ui;
}
