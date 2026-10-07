// bdc 0x089cf500 NetCharaMgrDestroy
#include "bdc.h"

/* Tears down the `CONetChara` manager: clears the active byte, destroys every character
   (`NetCharaDtor` with flags 3) and purges the list, frees `g_netCharaSlots`, destroys the lock
   and the list, frees `g_netCharaMgr` and finally `NetInviteDestroy`. Called by
   `NetAdhocDestroy`. */

void NetCharaMgrDestroy(void)
{
  NetCharaList *list;
  NetCharaListNode *node;
  NetCharaListNode *next;
  void *slots;

  g_netCharaMgrActive = 0;
  if (g_netCharaMgr != NULL) {
    CoreLockAcquire(g_netCharaMgr->lock);
    list = g_netCharaMgr->list;
    if (list != NULL && list->count > 0) {
      node = (NetCharaListNode *)NetCharaListFirst(list);
      while (node != NULL) {
        next = node->next;
        if (node->chara != NULL) {
          NetCharaDtor(node->chara, 3);
        }
        node = next;
      }
      NetCharaListPurgeRemoved(g_netCharaMgr->list);
    }
    slots = g_netCharaSlots;
    if (slots != NULL) {
      MemLock();
      MemFree(slots, NULL, 0);
      MemUnlock();
      g_netCharaSlots = NULL;
    }
    CoreLockRelease(g_netCharaMgr->lock);
    if (g_netCharaMgr->lock != NULL) {
      CoreLockDestroy(g_netCharaMgr->lock, 3);
      g_netCharaMgr->lock = NULL;
    }
    if (g_netCharaMgr->list != NULL) {
      NetCharaListDestroy((CoreList *)g_netCharaMgr->list, 3);
      g_netCharaMgr->list = NULL;
    }
    if (g_netCharaMgr != NULL) {
      MemLock();
      MemFree(g_netCharaMgr, NULL, 0);
      MemUnlock();
      g_netCharaMgr = NULL;
    }
  }
  NetInviteDestroy();
}
