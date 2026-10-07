// bdc 0x08a2f28c SndGroupHoldListPurgeRemoved
#include "bdc.h"

/* Walks the group hold list of the `SndGroupLoader` (`holds`, data = 8-byte `{groupId, ttl}`
   records) and unlinks every node flagged `removed`, freeing each one to the list's pool (or the
   heap) and decrementing `count`. Run after iteration finishes to complete the deferred removals of
   the remove function. */

void SndGroupHoldListPurgeRemoved(CoreList *list)
{
  CoreListNode *prev;
  CoreListNode *node;

  prev = list->sentinel;
  if (prev == NULL) {
    return;
  }
  node = prev->next;
  while (node != NULL) {
    if (node->removed == 0) {
      prev = node;
    } else {
      prev->next = node->next;
      node->next = NULL;
      if (list->pool != NULL && MemPoolFree(list->pool, node)) {
        node = NULL;
      }
      if (node != NULL) {
        MemLock();
        MemFree(node, NULL, 0);
        MemUnlock();
      }
      list->count = list->count - 1;
    }
    node = prev->next;
  }
}
