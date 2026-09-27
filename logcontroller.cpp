/**
 * @file logcontroller.cpp
 * @note ログ出力Controllerクラス
 **/


/**
 * インクルード各種
 */
#include "logcontroller.h"
#include "logworker.h"

/**
 * @brief シングルトンインスタンスの取得
 * @return インスタンス
 * @note (C++11以降、この実装はスレッドセーフです)
 */
LogController* LogController::instance()
{
    // 初回はインスタンスを生成、２回目以降は生成済のインスタンスを返す
    static LogController internalInstance;
    return &internalInstance;
}

/**
 * @brief コンストラクタ
 */
LogController::LogController()
    : QObject( NULL ),
    m_stopping( false ),
    m_worker( new LogWorker( this ) )
{
    // ワーカースレッドの開始
    this->m_worker->start();
}

/**
 * @brief デストラクタ
 */
LogController::~LogController()
{
    // ログ情報キューに残っている情報を出力する
    {
        QMutexLocker locker( &this->m_mutex );

        this->m_stopping = true;

        // wait()中のWorkerを起こす
        this->m_condition.wakeOne();
    }

    // Worker終了を待つ
    this->m_worker->wait();

    // ワーカースレッドを削除する
    delete this->m_worker;
    this->m_worker = nullptr;
}

/**
 * @brief debugログ出力リクエスト
 * @param message ログメッセージ
 */
void LogController::debug( const QString& message )
{
    // ログ情報キューへ格納する
    this->enqueue( LogLevel::Debug, message );
}

/**
 * @brief infoログ出力リクエスト
 * @param message ログメッセージ
 */
void LogController::info( const QString& message )
{
    // ログ情報キューへ格納する
    this->enqueue( LogLevel::Info, message );
}

/**
 * @brief warningログ出力リクエスト
 * @param message ログメッセージ
 */
void LogController::warning( const QString& message )
{
    // ログ情報キューへ格納する
    this->enqueue( LogLevel::Warning, message );
}

/**
 * @brief errorログ出力リクエスト
 * @param message ログメッセージ
 */
void LogController::error( const QString& message )
{
    // ログ情報キューへ格納する
    this->enqueue( LogLevel::Error, message );
}

/**
 * @brief ログ情報キューへログを格納する
 * @param level ログレベル
 * @param message ログメッセージ
 */
void LogController::enqueue( LogLevel level, const QString& message )
{
    // ログ情報の格納
    LogMessage log;
    log.timestamp = QDateTime::currentDateTime();
    log.level = level;
    log.message = message;

    // LogWorkerのスレッドと並行処理を考慮して
    // MutexLockerを使ってスレッドセーフな処理にする
    {
        QMutexLocker locker( &this->m_mutex );

        // 終了処理中なら追加しない
        if ( this->m_stopping )
        {
            return;
        }

        // ログ情報キューへ格納する
        this->m_queue.enqueue( log );

        // Workerを起こす
        this->m_condition.wakeOne();
    }
}

/**
 * @brief ログ有無判断と待ち処理
 * @param log 取得てきたログ内容
 * @return true:ログあり, false:ログなし
 * @note LogWorkerからログが格納されるまで待つ関数として使用する
 */
bool LogController::waitAndTakeLog( LogMessage& log )
{
    // LogWorkerのスレッドと並行処理を考慮して
    // MutexLockerを使ってスレッドセーフな処理にする
    QMutexLocker locker( &this->m_mutex );

    while ( this->m_queue.isEmpty() && !this->m_stopping )
    {
        // ログが追加されるまで待つ
        this->m_condition.wait(&m_mutex);
    }

    // 終了要求
    if ( this->m_stopping && this->m_queue.isEmpty())
    {
        return false;
    }

    // キューから1件取り出す
    log = this->m_queue.dequeue();

    return true;
}
