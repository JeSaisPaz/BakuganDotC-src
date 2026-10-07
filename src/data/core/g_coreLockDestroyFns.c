// bdc 0x08ac4f00 g_coreLockDestroyFns
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_coreLockDestroyFns = {
    { .pfn = (void *)CoreLockNoneOp }, { .pfn = (void *)CoreLockMutexDestroy },
    { .pfn = (void *)CoreLockLwMutexDestroy }, { .pfn = (void *)CoreLockSemaDestroy },
    { .pfn = (void *)CoreLockSpinDestroy },
};
