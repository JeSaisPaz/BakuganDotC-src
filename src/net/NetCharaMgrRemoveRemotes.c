// bdc 0x089cfaf4 NetCharaMgrRemoveRemotes
#include "bdc.h"

/* Destroys every net character except the first (local) one: marks each list node removed (`+8 =
   1`), runs `NetCharaDtor``(chara, 3)` and purges the list, under the manager lock. Called when a
   session launches (`NetPlayState3HostLaunch`, `NetPlayState6JoinLaunch`). */

void NetCharaMgrRemoveRemotes(void)
{
  NetCharaListNode *node;
  int count;
  int i;

  if (g_netCharaMgr != NULL && g_netCharaMgr->list != NULL) {
    CoreLockAcquire(g_netCharaMgr->lock);
    count = g_netCharaMgr->list->count;
    if (count > 0) {
      node = NetCharaListFirst(g_netCharaMgr->list);
      for (i = 0; i < count; i++) {
        if (i != 0) {
          node->removed = 1;
          if (node->chara != NULL) {
            NetCharaDtor(node->chara, 3);
          }
        }
        node = node->next;
      }
      NetCharaListPurgeRemoved(g_netCharaMgr->list);
    }
    CoreLockRelease(g_netCharaMgr->lock);
  }
}
