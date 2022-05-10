#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <doglistsql.h>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);



    QQmlApplicationEngine engine;  // движок для QML

    QQmlContext *context = engine.rootContext();    // получить корневой контекст модели


    //StudentList viewModel; // модель для приложения
    //context->setContextObject(&viewModel);          // установить в качестве корневой модели созданную модель для приложения

    doglistsql viewModel(&app); // модель для приложения
    context->setContextObject(&viewModel);


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
