// bdc 0x08a2fa5c SndBgmCmdListInsert
#include "bdc.h"

/* Adds `data` to the BGM command list: takes a node from the pool (else a 0x10-byte node from the
   low heap), stores the payload (`+0xc`) and `priority` (`+4`), and links it in front of the first
   node whose priority is strictly greater, scanning at most `count` nodes (appended at the end
   otherwise, so equal priorities keep creation order). Increments `count` and returns the new
   count, or -1 when no node could be allocated. Called with priority 1000 by `SndBgmCmdInit`. */

s32 SndBgmCmdListInsert(CoreList *list, SndBgmCmd *data, s32 priority)
{
  CoreListNode *node;
  CoreListNode *heapNode;
  CoreListNode *prev;
  CoreListNode *cur;
  bool fromLow;
  s32 count;
  s32 i;

  node = NULL;
  if (list->pool != NULL) {
    node = (CoreListNode *)MemPoolAlloc(list->pool);
  }
  if (node != NULL) {
    node->next = NULL;
    node->removed = 0;
  } else {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    heapNode = (CoreListNode *)MemAlloc(sizeof(CoreListNode), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (heapNode != NULL) {
      heapNode->next = NULL;
      heapNode->removed = 0;
      node = heapNode;
    }
  }
  if (node == NULL) {
    return -1;
  }
  node->data = data;
  node->priority = priority;
  prev = list->sentinel;
  count = list->count;
  if (prev != NULL) {
    cur = prev->next;
    for (i = 0; i < count; i++) {
      if (cur == NULL || priority < cur->priority) {
        break;
      }
      prev = cur;
      cur = prev->next;
    }
    prev->next = node;
    node->next = cur;
    count = list->count;
  }
  list->count = count + 1;
  return count + 1;
}
