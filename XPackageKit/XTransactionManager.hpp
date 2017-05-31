#ifndef XTransactionManager_hpp
#define XTransactionManager_hpp

#include <QObject>
#include <QVector>

#include "XTransaction.hpp"

class XTransactionManager : public QObject
{
    Q_OBJECT
public:
    explicit XTransactionManager(QObject *parent = nullptr);
    ~XTransactionManager();

    void setAutodeleteEnabled(bool enable);

    XTransaction *addRepository(const QString &repoName, const QVariantMap &details, QObject *parent = nullptr);
    XTransaction *removeRepository(const QString &repoName, QObject *parent = nullptr);
    XTransaction *setRepositoryEnabled(const QString &repoName, bool enable = true, QObject *parent = nullptr);
    XTransaction *refreshRepository(const QString &repoName, QObject *parent = nullptr);
    XTransaction *installPackage(const QString &packageName, QObject *parent = nullptr);
    XTransaction *removePackage(const QString &packageName, QObject *parent = nullptr);

signals:
    void started(XTransaction *transaction);

private:
    class Private;
    Private *d;

};

#endif // XTransactionManager_hpp
