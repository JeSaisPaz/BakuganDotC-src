// bdc 0x089cf684 NetCharaGetSentHeader
#include "bdc.h"

/* Copies the 0x34-byte local header (`outHdr`) of net character `index` into `out`, under the
   manager and character locks. Returns 1 when the character exists, 0 otherwise (also when the
   manager is missing/inactive/has no list or `out` is NULL). */

int NetCharaGetSentHeader(NetCharaPacketHeader *out, s32 index)
{
  NetChara *chara;
  int found = 0;

  if (g_netCharaMgr != NULL && g_netCharaMgrActive != 0 && out != NULL &&
      g_netCharaMgr->list != NULL) {
    CoreLockAcquire(g_netCharaMgr->lock);
    chara = NetCharaGetByIndex(index);
    if (chara != NULL) {
      CoreLockAcquire(chara->lock);
      *out = chara->outHdr;
      CoreLockRelease(chara->lock);
      found = 1;
    }
    CoreLockRelease(g_netCharaMgr->lock);
  }
  return found;
}
