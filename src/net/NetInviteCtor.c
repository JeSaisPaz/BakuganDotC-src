// bdc 0x089d2a30 NetInviteCtor
#include "bdc.h"

/* Constructor of the `CONetInvate` (sic) ad-hoc invitation object (0x78 bytes): `+0x0` a
   `CoreLock` LwMutex named "CONetInvate", bytes `+0x4`/`+0x5` cleared, words `+0x70`/`+0x74`
   cleared. Built by `NetInviteCreate`. */

NetInvite *NetInviteCtor(NetInvite *invite)
{
  bool fromLow;
  CoreLock *lock;
  CoreLock *result;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  lock = MemAlloc(0x38, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  result = NULL;
  if (lock != NULL) {
    CoreLockInit(lock, "CONetInvate", CORE_LOCK_LWMUTEX);
    result = lock;
  }
  invite->lock = result;
  invite->pendingA = 0;
  invite->pendingB = 0;
  invite->unk70 = 0;
  invite->unk74 = 0;
  return invite;
}
