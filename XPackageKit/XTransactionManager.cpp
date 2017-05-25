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
    detailsEx.insert("repoName", repoName);
    t->setRequestDetails(detailsEx);
    return t;
}

XTransaction *XTransactionManager::removeRepository(const QString &repoName, QObject *parent)
{
    XTransaction *t = d->createTransaction<XSsuTransaction>(XTransaction::RemoveRepoRequest, parent);
    t->setRequestDetails({{"repoName", repoName }});
    return t;
}

XTransaction *XTransactionManager::setRepositoryEnabled(const QString &repoName, bool enable, QObject *parent)
{
    XTransaction *t = d->createTransaction<XSsuTransaction>(XTransaction::SetRepoEnabledRequest, parent);
    t->setRequestDetails({{"repoName", repoName }, {"enable", enable}});
    return t;
}

XTransaction *XTransactionManager::refreshRepository(const QString &repoName, QObject *parent)
{
    XTransaction *t = d->createTransaction<XPackageKitTransaction>(XTransaction::RefreshRepoRequest, parent);
    t->setRequestDetails({{"repoName", repoName }});
    return t;
}

XTransaction *XTransactionManager::installPackage(const QString &packageName, QObject *parent)
{
    XTransaction *t = d->createTransaction<XPackageKitTransaction>(XTransaction::InstallRequest, parent);
    t->setRequestDetails({{"packageName", packageName }});
    return t;
}
