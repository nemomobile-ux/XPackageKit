#include "XTransactionNamespace.hpp"

Q_LOGGING_CATEGORY(categoryXPackageKit, "omp.xpackagekit", QtDebugMsg)

QDebug XTransactionNamespace::info(const char *file, int line, const char *func, QObject *object)
{
    return QMessageLogger(file, line, func).info(categoryXPackageKit) << QStringLiteral("[%1]").arg(object->objectName());
}

QDebug XTransactionNamespace::debug(const char *file, int line, const char *func, QObject *object)
{
    return QMessageLogger(file, line, func).debug(categoryXPackageKit) << QStringLiteral("[%1]").arg(object->objectName());
}

QDebug XTransactionNamespace::warning(const char *file, int line, const char *func, QObject *object)
{
    return QMessageLogger(file, line, func).warning(categoryXPackageKit) << QStringLiteral("[%1]").arg(object->objectName());
}

QDebug XTransactionNamespace::critical(const char *file, int line, const char *func, QObject *object)
{
    return QMessageLogger(file, line, func).critical(categoryXPackageKit) << QStringLiteral("[%1]").arg(object->objectName());
}
