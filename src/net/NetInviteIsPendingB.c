// bdc 0x089d2c18 NetInviteIsPendingB
#include "bdc.h"

/* Returns 1 when the invite object's pending flag (`pendingB`, byte `+5`) is set and `mac` is NULL or
   equals the stored MAC (`macB`); else 0. Evaluated under the invite lock. */

int NetInviteIsPendingB(NetInvite *invite, u8 *mac)
{
  int result = 0;
  int i;

  CoreLockAcquire(invite->lock);
  if (invite->pendingB != 0) {
    result = 1;
    if (mac != NULL) {
      for (i = 0; i < 6; i++) {
        if (invite->macB[i] != mac[i]) {
          result = 0;
          break;
        }
      }
    }
  }
  CoreLockRelease(invite->lock);
  return result;
}
