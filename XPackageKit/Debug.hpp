#ifndef XPACKAGEKIT_DEBUG_HPP
#define XPACKAGEKIT_DEBUG_HPP

#include <QDebug>

#define xLogItem QStringLiteral("[%1]").arg(objectName())

#define xDebug() QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug() << xLogItem
#define xInfo() QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).info() << xLogItem
#define xWarning() QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).warning() << xLogItem
#define xCritical() QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).critical() << xLogItem
#define xFatal() QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).fatal() << xLogItem

#endif // XPACKAGEKIT_DEBUG_HPP
