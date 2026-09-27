/**
 * @file logcontroller.h
 * @note ログ出力Controllerクラス
 **/

#ifndef LOGCONTROLLER_H
#define LOGCONTROLLER_H

/**
 * インクルード各種
 */
#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

#include "logtypes.h"


/**
 * クラス定義
 */
class LogWorker;


/**
 * @brief LogControllerクラス
 */
class LogController : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief シングルトンインスタンスの取得
     * @return インスタンス
     */
    static LogController* instance();


private:
    /**
     * @brief コンストラクタ
     */
    LogController();

    /**
     * @brief デストラクタ
     */
    ~LogController();

    /**
     * @brief コピーと代入の禁止（シングルトンを保証）
     */
    LogController( const LogController& ) = delete;
    LogController& operator=( const LogController& ) = delete;
    LogController( LogController&& ) = delete;
    LogController& operator=( LogController&& ) = delete;

public:
    /**
     * @brief debugログ出力リクエスト
     * @param message ログメッセージ
     */
    void debug( const QString& message );

    /**
     * @brief infoログ出力リクエスト
     * @param message ログメッセージ
     */
    void info( const QString& message );

    /**
     * @brief warningログ出力リクエスト
     * @param message ログメッセージ
     */
    void warning( const QString& message );

    /**
     * @brief errorログ出力リクエスト
     * @param message ログメッセージ
     */
    void error( const QString& message );

    /**
     * @brief ログ有無判断と待ち処理
     * @param log 取得てきたログ内容
     * @return true:ログあり, false:ログなし
     * @note LogWorkerからログが格納されるまで待つ関数として使用する
     */
    bool waitAndTakeLog(LogMessage& log);


private:
    /**
     * @brief ログ情報キューへログを格納する
     * @param level ログレベル
     * @param message ログメッセージ
     */
    void enqueue( LogLevel level, const QString& message );


private:
    /**
     * メンバ変数
     */
    QQueue<LogMessage> m_queue;     // ログ情報キュー

    QMutex m_mutex;                 // スレッドセーフ処理のためのMutex
    QWaitCondition m_condition;     // ワーカースレッドのためのWaitCondition

    bool m_stopping;                // 止まっているかどうかのフラグ

    LogWorker* m_worker;            // LogWorkerクラス


};

#endif // LOGCONTROLLER_H
