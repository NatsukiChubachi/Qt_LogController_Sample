/**
 * @file logworker.cpp
 * @note ログ出力Workerクラス
 **/


/**
 * インクルード各種
 */
#include "logworker.h"
#include "logcontroller.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>


/**
 * @brief コンストラクタ
 * @param parent 親クラス
 */
LogWorker::LogWorker( QObject* parent )
    : QThread( parent )
{

}

/**
 * @brief ワーカースレッドメイン処理
 */
void LogWorker::run()
{
    while( true )
    {
        LogMessage log;

        // Controllerからログを1件取得する
        if ( !LogController::instance()->waitAndTakeLog( log ) )
        {
            // falseの場合は終了要求
            break;
        }

        // Mutexを解放した状態でファイルへ書き込む
        this->writeLog( log );
    }
}

/**
 * @brief ログをファイルへ出力する
 * @param log 出力内容
 */
void LogWorker::writeLog( const LogMessage& log )
{
    // ログ出力ディレクトリ
    QString logDirectory =
        QCoreApplication::applicationDirPath()
        + "/log";

    QDir dir;

    if ( !dir.exists( logDirectory ) )
    {
        // ディレクトリの作成
        dir.mkpath( logDirectory );
    }

    // ログ出力ファイル
    QString filePath =
        logDirectory + "/application.log";

    // ログファイルオープン
    QFile file( filePath );
    if ( !file.open(
            QIODevice::WriteOnly |
            QIODevice::Append |
            QIODevice::Text
            ) )
    {
        return;
    }

    // ログ出力
    QTextStream stream( &file );
    stream << log.timestamp.toString( "yyyy-MM-dd hh:mm:ss.zzz" )
           << " ["
           << levelToString( log.level )
           << "] "
           << log.message
           << Qt::endl;
    file.close();
}

/**
 * @brief ログレベルに応じた文字列を返す
 * @param level ログレベル
 * @return ログレベルに応じた文字列
 */
QString LogWorker::levelToString( LogLevel level ) const
{
    // ログレベルに応じた文字列を返す
    QString result = "";

    switch ( level )
    {
    case LogLevel::Debug:
        result = "DEBUG";
        break;

    case LogLevel::Info:
        result = "INFO";
        break;

    case LogLevel::Warning:
        result = "WARNING";
        break;

    case LogLevel::Error:
        result = "ERROR";
        break;

    default:
        result = "UNKNOWN";
        break;
    }

    return result;
}

