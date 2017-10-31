#ifndef XPROCESSTRANSACTION_HPP
#define XPROCESSTRANSACTION_HPP

#include "XTransaction.hpp"

#include <QProcess>

class XProcessTransaction : public XTransaction
{
    Q_OBJECT
public:
    explicit XProcessTransaction(QObject *parent = nullptr);
    QString program() const;
    QStringList arguments() const;

protected:
    void startEvent() override;

private slots:
    void onProcessFinished(int exitCode);
    void onProcessStateChanged(QProcess::ProcessState state);

private:
    QProcess *m_process;

};

#endif // XPROCESSTRANSACTION_HPP
