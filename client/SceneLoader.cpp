#include "SceneLoader.h"

SceneLoader::SceneLoader(QObject* parent)
    : QObject{ parent }
{

}

void SceneLoader::add(int i, QWidget* w)
{
    stack->insertWidget(i, w);
}

void SceneLoader::show(int i)
{
    stack->setCurrentIndex(i);
    stack->showMaximized();
}

void SceneLoader::init()
{
    stack = new QStackedWidget;
}

void SceneLoader::delete1(MainWindow1* mw1)
{
    stack->removeWidget(mw1);
    delete mw1;
}

SceneLoader* SceneLoader::sl = new SceneLoader;
