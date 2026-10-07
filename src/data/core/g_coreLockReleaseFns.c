// bdc 0x08ac4f50 g_coreLockReleaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_coreLockReleaseFns = {
    { .pfn = (void *)CoreLockNoneOp }, { .pfn = (void *)CoreLockMutexRelease },
    { .pfn = (void *)CoreLockLwMutexRelease }, { .pfn = (void *)CoreLockSemaRelease },
    { .pfn = (void *)CoreLockSpinRelease },
};
