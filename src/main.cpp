/*
Тестовое задание
Переводчик из азбуки Морзе и обратно
*/

#include <QtWidgets/QApplication>
#include "documenthandler.h"
#include <QtQml/QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qmlRegisterType<DocumentHandler>("org.qtproject.example", 1, 0, "DocumentHandler");
    QQmlApplicationEngine engine(QUrl("qrc:/qml/main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
