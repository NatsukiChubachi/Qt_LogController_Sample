/**
 * @file logworker.h
 * @note ログ出力Workerクラス
 **/
#ifndef LOGWORKER_H
#define LOGWORKER_H


/**
 * インクルード各種
 */
#include <QThread>
#include <QString>

#include "logtypes.h"


/**
 * クラス定義
 */
class LogController;


/**
 * @brief LogWorkerクラス
 */
class LogWorker : public QThread
{
    Q_OBJECT

public:
    /**
     * @brief コンストラクタ
     * @param parent 親クラス
     */
    explicit LogWorker( QObject* parent = nullptr );

    /**
     * @brief デストラクタ
     */
    ~LogWorker() override = default;


protected:
    /**
     * @brief ワーカースレッドメイン処理
     */
    void run() override;


private:
    /**
     * @brief ログをファイルへ出力する
     * @param log 出力内容
     */
    void writeLog( const LogMessage& log );

    /**
     * @brief ログレベルに応じた文字列を返す
     * @param level ログレベル
     * @return ログレベルに応じた文字列
     */
    QString levelToString( LogLevel level ) const;


};

#endif // LOGWORKER_H

