// bdc 0x089d2adc NetInviteDtor
#include "bdc.h"

/* Destructor of the `CONetInvate` object: destroys its `CoreLock` (`+0`) and frees the object
   when bit 0 of `flags` is set. */

void NetInviteDtor(NetInvite *invite, u32 flags)
{
  if (invite != NULL) {
    if (invite->lock != NULL) {
      CoreLockDestroy(invite->lock, 3);
      invite->lock = NULL;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(invite, NULL, 0);
      MemUnlock();
    }
  }
}
