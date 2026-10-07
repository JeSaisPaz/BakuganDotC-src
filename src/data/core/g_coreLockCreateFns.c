// bdc 0x08ac4ed8 g_coreLockCreateFns
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_coreLockCreateFns = {
    { .pfn = (void *)CoreLockNoneOp }, { .pfn = (void *)CoreLockMutexCreate },
    { .pfn = (void *)CoreLockLwMutexCreate }, { .pfn = (void *)CoreLockSemaCreate },
    { .pfn = (void *)CoreLockSpinCreate },
};
