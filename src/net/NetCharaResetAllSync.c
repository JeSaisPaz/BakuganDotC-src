// bdc 0x089cff64 NetCharaResetAllSync
#include "bdc.h"

/* Resets the lock-step state of every net character (`NetCharaResetSync`), zeroes
   `g_netCharaSlots` and sets the NetPlay manager's word `+0xe8` to -1. Called when a net battle
   starts or ends (`BtlMainTaskCtor`, `BtlMainTeardown`, `NetBattleSyncTaskUpdate`,
   `UiPauseUpdate`, …). */
void NetCharaResetAllSync(void)
{
    NetCharaListNode *node;
    NetChara *chara;
    NetPlay *play;

    if (g_netCharaMgr != NULL && g_netCharaMgr->lock != NULL) {
        CoreLockAcquire(g_netCharaMgr->lock);
        node = NetCharaListFirst(g_netCharaMgr->list);
        if (node != NULL) {
            chara = node->chara;
            while (1) {
                node = node->next;
                if (chara != NULL) {
                    NetCharaResetSync(chara);
                }
                if (node == NULL) {
                    break;
                }
                chara = node->chara;
            }
        }
        memset(g_netCharaSlots, 0, 0x3c0);
        CoreLockRelease(g_netCharaMgr->lock);
        play = NetPlayGetManager();
        play->maxFrameSeen = -1;
    }
}
