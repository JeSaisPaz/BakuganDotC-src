// bdc 0x089bb7b8 CoreLockNoneOp
#include "bdc.h"

/* Empty operation of the `CORE_LOCK_NONE` backend of `CoreLock` (backend table
   `0x08ac4ed0`, `CoreLockType`): used for its create, destroy, acquire and release entries. */
void CoreLockNoneOp(CoreLock *lock)
{
    (void)lock;
}
