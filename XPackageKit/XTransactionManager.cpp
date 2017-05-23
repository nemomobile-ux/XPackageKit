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
    T *createTransaction(XTransaction::RequestType request, const QString &name, QObject *parent)
    {
        if (parent == nullptr) {
            parent = q;
        }

        T *t = new T(request, name, parent);
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
    XTransaction *t = d->createTransaction<XSsuTransaction>(XTransaction::AddRepoRequest, repoName, parent);
    t->setRequestDetails(details);
    return t;
}

XTransaction *XTransactionManager::removeRepository(const QString &repoName, QObject *parent)
{
    return d->createTransaction<XSsuTransaction>(XTransaction::RemoveRepoRequest, repoName, parent);
}

XTransaction *XTransactionManager::setRepositoryEnabled(const QString &repoName, bool enable, QObject *parent)
{
//    XTransaction *t = d->createTransaction<XPackageKitTransaction>(XTransaction::SetRepoEnabledRequest, repoName, parent);
    XTransaction *t = d->createTransaction<XSsuTransaction>(XTransaction::SetRepoEnabledRequest, repoName, parent);
    t->setRequestDetails({{"enable", enable}});
    return t;
}

XTransaction *XTransactionManager::refreshRepository(const QString &repoName, QObject *parent)
{
    return d->createTransaction<XPackageKitTransaction>(XTransaction::RefreshRepoRequest, repoName, parent);
}

XTransaction *XTransactionManager::installPackage(const QString &packageName, QObject *parent)
{
    return d->createTransaction<XPackageKitTransaction>(XTransaction::InstallRequest, packageName, parent);
}
