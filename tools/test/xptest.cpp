#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QTimer>

#include <QMap>

#include "XPackageKit/XTransaction.hpp"
#include "XPackageKit/XChainTransaction.hpp"
#include "XPackageKit/XTransactionManager.hpp"

#include "XTestUpdate.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("XPackage test"));

    qDebug() << "### XTestUpdate statred";

    XTestUpdate *test = new XTestUpdate(QStringLiteral("repo-meta-package"));
    QTimer::singleShot(0, test, &XTestUpdate::start);
    QObject::connect(test, &XTestUpdate::packagesToBeInstalled, [](const QStringList &packageNames) {
        qDebug() << "### ### packagesToBeInstalled:" << packageNames;
    });
    QObject::connect(test, &XTestUpdate::packagesInstalled, [](const QStringList &packageNames) {
        qDebug() << "### ### packagesInstalled:" << packageNames;
    });
    QObject::connect(test, &XTestUpdate::packagesToBeUpdated, [](const QStringList &packageNames) {
        qDebug() << "### ### packagesToBeUpdated:" << packageNames;
    });
    QObject::connect(test, &XTestUpdate::packagesUpdated, [](const QStringList &packageNames) {
        qDebug() << "### ### packagesUpdated:" << packageNames;
    });
    QObject::connect(test, &XTestUpdate::packagesToBeRemoved, [](const QStringList &packageNames) {
        qDebug() << "### ### packagesToBeRemoved:" << packageNames;
    });
    QObject::connect(test, &XTestUpdate::packagesRemoved, [](const QStringList &packageNames) {
        qDebug() << "### ### packagesRemoved:" << packageNames;
    });
    QObject::connect(test, &XTestUpdate::finished, [](XTransaction *tx) {
        qDebug() << "### XTestUpdate finished" << tx->results() << tx->errorDetails();
        qApp->quit();
    });
    QObject::connect(test, &XTestUpdate::progressChanged, [](XTransaction *, uint progress) {
        qDebug() << "### XTestUpdate progress:" << progress;
    });

    return app.exec();
}
