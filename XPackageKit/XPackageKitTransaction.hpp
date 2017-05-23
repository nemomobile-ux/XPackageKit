#ifndef XPackageKitTransaction_hpp
#define XPackageKitTransaction_hpp

#include "XTransaction.hpp"

#include <Transaction>

class XPackageKitTransaction : public XTransaction
{
    Q_OBJECT
public:
    explicit XPackageKitTransaction(RequestType type, const QString &name, QObject *parent = nullptr);

    QString packageName() const { return m_name; }

    struct PackageArgs {
        PackageKit::Transaction::Info info;
        QString packageID;
        QString summary;
    };

    struct ErrorStruct {
        PackageKit::Transaction::Error error;
        QString details;
    };

    PackageArgs getExactSearchResult() const;

signals:
    void packageStatusChanged(const QString &packageName, PackageStatus);

protected slots:
    void onSearchResult(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);
    void onSearchTransactionFinished(PackageKit::Transaction::Exit status, uint runtime);

    void onTransactionErrorCode(PackageKit::Transaction::Error error, const QString &details);
    void onGenericTransactionFinished(PackageKit::Transaction::Exit status, uint runtime);

    void onStatusChanged();

    void doInstall(const QString &packageId);
    void doRemove(const QString &packageId);
    void doUpdate(const QString &packageId);

protected:
    void search();
    void refresh(const QString &repoName);
    void setRepoEnabled(const QString &repoName, bool enable);

protected:
    void startEvent() override;

    QString m_name;
    QVector<PackageArgs> m_searchResult;
    QVector<ErrorStruct> m_errors;

};

#endif // XPackageKitTransaction_hpp
