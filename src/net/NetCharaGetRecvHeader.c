// bdc 0x089cf7a0 NetCharaGetRecvHeader
#include "bdc.h"

/* Copies the 0x34-byte header last received from net character `index` (`inHdr`, `+0x40..+0x73`)
   into `out`, under the manager lock and the character's lock. Returns 1 when the character
   exists and the header was copied; 0 when the manager is missing/inactive/has no list, `out` is
   NULL, or there is no character `index`. */
int NetCharaGetRecvHeader(NetCharaPacketHeader *out, s32 index)
{
    NetChara *chara;
    int found = 0;

    if (g_netCharaMgr != NULL && g_netCharaMgrActive != 0 && out != NULL &&
        g_netCharaMgr->list != NULL) {
        CoreLockAcquire(g_netCharaMgr->lock);
        chara = NetCharaGetByIndex(index);
        if (chara != NULL) {
            CoreLockAcquire(chara->lock);
            *out = chara->inHdr;
            CoreLockRelease(chara->lock);
            found = 1;
        }
        CoreLockRelease(g_netCharaMgr->lock);
    }
    return found;
}
