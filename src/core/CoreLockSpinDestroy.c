// bdc 0x089bb9b4 CoreLockSpinDestroy
#include "bdc.h"

/* `destroy` of the `CORE_LOCK_SPIN` backend of `CoreLock` (backend table `0x08ac4ed0`,
   `CoreLockType`): empty. */
void CoreLockSpinDestroy(CoreLock *lock)
{
    (void)lock;
}
