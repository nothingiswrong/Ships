#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "battlefieldmodel.h"
#include <appcontroller.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;


    AppController controller;
    engine.rootContext()->setContextProperty("controller", &controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("Ships2", "Main");

    BattleFieldModel model;


    return QCoreApplication::exec();
}
