#ifndef XTRANSACTIONNAMESPACE_HPP
#define XTRANSACTIONNAMESPACE_HPP

#include <QObject>

class XTransactionNamespace
{
    Q_GADGET
public:
    enum Filter {
        FilterNone          = 1 << 0,
        FilterInstalled     = 1 << 1,
        FilterNotInstalled  = 1 << 2,
        FilterBasename      = 1 << 3,
        FilterNotBasename   = 1 << 4,
        FilterNewest        = 1 << 5,
        FilterNotNewest     = 1 << 6,
    };
    Q_DECLARE_FLAGS(Filters, Filter)

    enum TransactionFlag {
        TransactionFlagNone         = 1 << 0,
        TransactionFlagOnlyTrusted  = 1 << 1,
        TransactionFlagSimulate     = 1 << 2,
        TransactionFlagOnlyDownload = 1 << 3,
    };
    Q_DECLARE_FLAGS(TransactionFlags, TransactionFlag)

    enum TransactionStatus {
        PackageStatusUnknown = 0,
        PackageStatusWait,
        PackageStatusSetup,
        PackageStatusRunning,
        PackageStatusQuery,
        PackageStatusInfo,
        PackageStatusRemove,
        PackageStatusRefreshCache,
        PackageStatusDownload,
        PackageStatusInstall,
        PackageStatusUpdate,
        PackageStatusCleanup,
        PackageStatusObsolete,
        PackageStatusDepResolve,
        PackageStatusSigCheck,
        PackageStatusTestCommit,
        PackageStatusCommit,
        PackageStatusRequest,
        PackageStatusFinished,
        PackageStatusCancel,
        PackageStatusDownloadRepository,
        PackageStatusDownloadPackagelist,
        PackageStatusDownloadFilelist,
        PackageStatusDownloadChangelog,
        PackageStatusDownloadGroup,
        PackageStatusDownloadUpdateinfo,
        PackageStatusRepackaging,
        PackageStatusLoadingCache,
        PackageStatusScanApplications,
        PackageStatusGeneratePackageList,
        PackageStatusWaitingForLock,
        PackageStatusWaitingForAuth,
        PackageStatusScanProcessList,
        PackageStatusCheckExecutableFiles,
        PackageStatusCheckLibraries,
        PackageStatusCopyFiles,

        RepoStatusIdle = PackageStatusUnknown,
        RepoStatusWorking = PackageStatusRunning,
        RepoStatusCompleted = PackageStatusFinished,

        ProcessStatusNotRunning = 2000,
        ProcessStatusStarting,
        ProcessStatusRunning,
        ProcessStatusFinished,

        TransactionStatusUnknown = 100000
    };

#ifndef Q_QDOC
    Q_ENUM(TransactionStatus)

    Q_FLAG(Filters)
    Q_FLAG(TransactionFlags)
#endif

};

Q_DECLARE_OPERATORS_FOR_FLAGS(XTransactionNamespace::Filters)
Q_DECLARE_OPERATORS_FOR_FLAGS(XTransactionNamespace::TransactionFlags)

#endif // XTRANSACTIONNAMESPACE_HPP
