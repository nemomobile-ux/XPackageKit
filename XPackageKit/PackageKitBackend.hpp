#ifndef PACKAGEKITBACKEND_HPP
#define PACKAGEKITBACKEND_HPP

#include <QObject>

#include <Transaction>

class PackageKitBackend : public QObject
{
    Q_OBJECT
public:
    static PackageKit::Transaction *mkResolveTransaction(const QString &resolveTerm, PackageKit::Transaction::Filters filters = PackageKit::Transaction::FilterNone);
    static PackageKit::Transaction *mkSearchNameTransaction(const QString &searchTerm, PackageKit::Transaction::Filters filters = PackageKit::Transaction::FilterNone);
    static PackageKit::Transaction *mkInstallPackageTransaction(const QString &packageID, PackageKit::Transaction::TransactionFlags flags = PackageKit::Transaction::TransactionFlagNone);
    static PackageKit::Transaction *mkRemovePackageTransaction(const QString &packageID, bool allowDeps = false, bool autoRemove = false, PackageKit::Transaction::TransactionFlags flags = PackageKit::Transaction::TransactionFlagNone);
    static PackageKit::Transaction *mkGetFilesTransaction(const QString &packageIDs);
    static PackageKit::Transaction *mkSetRepoEnabledTransaction(const QString &repoName, bool enabled);
    static PackageKit::Transaction *mkRefreshRepoTransaction(const QString &repoName);

};

#endif // PACKAGEKITBACKEND_HPP
