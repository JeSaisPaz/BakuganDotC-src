// bdc 0x089bb9ac CoreLockSpinCreate
#include "bdc.h"

/* `create` of the `CORE_LOCK_SPIN` backend of `CoreLock` (backend table `0x08ac4ed0`,
   `CoreLockType`): empty, the spin lock needs no kernel object. */
void CoreLockSpinCreate(CoreLock *lock)
{
    (void)lock;
}
