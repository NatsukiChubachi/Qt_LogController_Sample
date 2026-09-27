/**
 * @file logtypes.h
 * @note ログ情報定義ヘッダファイル
 **/

#ifndef LOGTYPES_H
#define LOGTYPES_H

#include <QDateTime>
#include <QString>

/**
 * @brief ログレベル定義
 */
enum class LogLevel
{
    Debug = 0,  // Debug
    Info,       // Information
    Warning,    // Warning
    Error       // Error
};

/**
 * @brief ログ情報キューへ格納する１件の情報構造体
 */
struct LogMessage
{
    QDateTime timestamp;    // タイムスタンプ
    LogLevel level;         // ログレベル
    QString message;        // ログメッセージ
};

#endif // LOGTYPES_H

