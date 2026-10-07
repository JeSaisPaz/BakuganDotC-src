// bdc 0x08ac4f28 g_coreLockAcquireFns
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_coreLockAcquireFns = {
    { .pfn = (void *)CoreLockNoneOp }, { .pfn = (void *)CoreLockMutexAcquire },
    { .pfn = (void *)CoreLockLwMutexAcquire }, { .pfn = (void *)CoreLockSemaAcquire },
    { .pfn = (void *)CoreLockSpinAcquire },
};
