// bdc 0x089d2b84 NetInviteIsPendingA
#include "bdc.h"

/* Returns 1 when the invite object's pending flag (`pendingA`, byte `+4`) is set and `mac` is NULL or
   equals the stored MAC (`macA`); else 0. Evaluated under the invite lock. */

int NetInviteIsPendingA(NetInvite *invite, u8 *mac)
{
  int result = 0;
  int i;

  CoreLockAcquire(invite->lock);
  if (invite->pendingA != 0) {
    result = 1;
    if (mac != NULL) {
      for (i = 0; i < 6; i++) {
        if (invite->macA[i] != mac[i]) {
          result = 0;
          break;
        }
      }
    }
  }
  CoreLockRelease(invite->lock);
  return result;
}
