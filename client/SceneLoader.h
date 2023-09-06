#pragma once

#include <QObject>
#include <QStackedWidget>
#include "MainWindow1.h"

class SceneLoader  : public QObject
{
	Q_OBJECT

private:
    SceneLoader(const SceneLoader& sl) = delete;
    SceneLoader operator=(const SceneLoader& sl) = delete;
    explicit SceneLoader(QObject* parent = nullptr);
    QStackedWidget* stack = nullptr;
    static SceneLoader* sl;

public:
    void add(int i, QWidget* w);
    void show(int i);
    static SceneLoader* getInstance() { return sl; }

public slots:
    void init();
    void delete1(MainWindow1* mw1);
};
