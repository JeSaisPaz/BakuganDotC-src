// bdc 0x089bba58 CoreLockSpinRelease
#include "bdc.h"

/* `release` of the `CORE_LOCK_SPIN` backend of the `CoreLock` backend table (`CoreLockType`):
   decrements `recursion` and, when it reaches 0, clears the owner (`id` = -1); always decrements
   `lockCount`. */
void CoreLockSpinRelease(CoreLock *lock)
{
    lock->recursion--;
    if (lock->recursion == 0) {
        lock->id = -1;
    }
    lock->lockCount--;
}
