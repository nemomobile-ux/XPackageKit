#include "XTransactionManager.hpp"

#include "XPackageKitTransaction.hpp"
#include "XSsuTransaction.hpp"

class XTransactionManager::Private
{
public:
    Private(XTransactionManager *parent) :
        q(parent),
        m_autodelete(false)
    {
    }

    template<typename T>
    T *createTransaction(XTransaction::RequestType request, QObject *parent)
    {
        if (parent == nullptr) {
            parent = q;
        }

        T *t = new T(request, parent);
        if (m_autodelete) {
            connect(t, &XTransaction::finished, t, &QObject::deleteLater);
        }
        return t;
    }

    XTransactionManager *q;
    bool m_autodelete;
};

XTransactionManager::XTransactionManager(QObject *parent) :
    QObject(parent),
    d(new Private(this))
{
}

XTransactionManager::~XTransactionManager()
{
    delete d;
}

void XTransactionManager::setAutodeleteEnabled(bool enable)
{
    d->m_autodelete = enable;
}

XTransaction *XTransactionManager::addRepository(const QString &repoName, const QVariantMap &details, QObject *parent)
{
    XTransaction *t = d->createTransaction<XSsuTransaction>(XTransaction::AddRepoRequest, parent);
    QVariantMap detailsEx = details;
    detailsEx.insert(QStringLiteral("repoName"), repoName);
    t->setRequestDetails(detailsEx);
    return t;
}

XTransaction *XTransactionManager::removeRepository(const QString &repoName, QObject *parent)
{
    XTransaction *t = d->createTransaction<XSsuTransaction>(XTransaction::RemoveRepoRequest, parent);
    t->setRequestDetails({{QStringLiteral("repoName"), repoName }});
    return t;
}

XTransaction *XTransactionManager::setRepositoryEnabled(const QString &repoName, bool enable, QObject *parent)
{
    XTransaction *t = d->createTransaction<XSsuTransaction>(XTransaction::SetRepoEnabledRequest, parent);
    t->setRequestDetails({{QStringLiteral("repoName"), repoName }, {QStringLiteral("enable"), enable}});
    return t;
}

XTransaction *XTransactionManager::refreshRepository(const QString &repoName, QObject *parent)
{
    XTransaction *t = d->createTransaction<XPackageKitTransaction>(XTransaction::RefreshRepoRequest, parent);
    t->setRequestDetails({{QStringLiteral("repoName"), repoName }});
    return t;
}

XTransaction *XTransactionManager::installPackage(const QString &packageName, QObject *parent)
{
    XTransaction *t = d->createTransaction<XPackageKitTransaction>(XTransaction::InstallRequest, parent);
    t->setRequestDetails({{QStringLiteral("packageName"), packageName }});
    return t;
}

/**
 * Create a transaction to remove a package \a packageName.
 * Well-known extra request details are booleans allowDeps and autoRemove.
 *
 * \return The transaction for given action and parameters.
 */
XTransaction *XTransactionManager::removePackage(const QString &packageName, QObject *parent)
{
    XTransaction *t = d->createTransaction<XPackageKitTransaction>(XTransaction::RemoveRequest, parent);
    t->setRequestDetails({{QStringLiteral("packageName"), packageName }});
    return t;
}
