/**
 * @file main.cpp
 * @note ログ出力Controllerクラス
 **/


/**
 * インクルード各種
 */
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>

#include "logcontroller.h"


/**
 * @brief メイン関数
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // ログ出力
    LogController::instance()->info( "アプリケーション起動" );

    // 言語設定
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Qt_LogController_Sample_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    // アプリケーション開始
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/Qt_LogController_Sample/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    // ログ出力
    LogController::instance()->info( "アプリケーション終了" );

    return app.exec();
}
