#include "MainWindow1.h"
#include "MainWindow2.h"
#include "sceneloader.h"
#include "MainWindow3.h"
#include "MainWindow4.h"
#include "MainWindow5.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    MainWindow1* w1 = new MainWindow1(&w);
    MainWindow2* w2 = new MainWindow2(&w);
    MainWindow3* w3 = new MainWindow3(&w);
    MainWindow4* w4 = new MainWindow4(&w);
    MainWindow5* w5 = new MainWindow5(&w);
    auto loader = SceneLoader::getInstance();
    loader->add(0, w2);
    loader->add(1, w3);
    loader->add(2, w4);
    loader->add(3, w5);
    loader->add(4, w1);
    loader->show(4);

    return a.exec();
}
