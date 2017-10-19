#ifndef XTransactionManager_hpp
#define XTransactionManager_hpp

#include <QDebug>
#include <QObject>
#include <QVector>

#include "XChainTransaction.hpp"
#include "XTransaction.hpp"
#include "XTransactionNamespace.hpp"

class XTransactionManager : public QObject
{
    Q_OBJECT
public:
    static void setDefaultParent(QObject *parent);
    static QObject *defaultParent();

    static void setAutodelete(bool autodelete);
    static bool isAutodelete();

    static XTransaction *addRepository(const QString &repoName, const QVariantHash &details,
                                       QObject *parent = nullptr);

    static XTransaction *removeRepository(const QString &repoName,
                                          QObject *parent = nullptr);

    static XTransaction *setRepositoryEnabled(const QString &repoName, bool enable = true,
                                              QObject *parent = nullptr);

    static XTransaction *refreshRepository(const QString &repoName, bool force = false,
                                           QObject *parent = nullptr);

    static XTransaction *refreshCache(bool force = false,
                                      QObject *parent = nullptr);

    static XTransaction *installPackages(const QStringList &packageNames,
                                         XTransactionNamespace::TransactionFlags transactionFlags = XTransactionNamespace::TransactionFlagNone,
                                         QObject *parent = nullptr);

    static XTransaction *removePackages(const QStringList &packageNames,
                                        QObject *parent = nullptr);

    static XTransaction *getUpdates(XTransactionNamespace::Filters filters = XTransactionNamespace::FilterNone,
                                    QObject *parent = nullptr);

    static XTransaction *resolve(const QStringList &packageNames,
                                 XTransactionNamespace::Filters filters = XTransactionNamespace::FilterNone,
                                 QObject *parent = nullptr);

    static XTransaction *getDepends(const QStringList &packageIds,
                                    XTransactionNamespace::Filters filters = XTransactionNamespace::FilterNone,
                                    QObject *parent = nullptr);

    static XTransaction *updatePackages(const QStringList &packageIds,
                                        XTransactionNamespace::TransactionFlags transactionFlags = XTransactionNamespace::TransactionFlagNone,
                                        QObject *parent = nullptr);

    static XTransaction *downloadPackages(const QStringList &packageIds,
                                          bool storeInCache = false,
                                          QObject *parent = nullptr);

    static XTransaction *processTransaction(const QString &program,
                                            const QStringList &arguments,
                                            QObject *parent = nullptr);

    static XChainTransaction *createChain(const QVector<XTransaction*> &transactions = {}, QObject *parent = nullptr);

    static QString packageName(const QString &packageId);
    static QString packageVersion(const QString &packageId);
    static QString packageArch(const QString &packageId);
    static QString packageData(const QString &packageId);

    static bool isInstalled(const QString &packageId);
};

#endif // XTransactionManager_hpp
