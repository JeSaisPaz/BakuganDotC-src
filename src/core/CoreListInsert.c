// bdc 0x08a2d878 CoreListInsert
#include "bdc.h"

/* Inserts into a `CoreList`: takes a node from `pool` (or 16 bytes from the low heap), stores
   `data` and `priority`, and links it after the last of the first `count` nodes whose priority
   is `<=` `priority` (equal keys keep insertion order). Returns the new `count`, or -1 when no
   node could be allocated. A list without a sentinel still gets its count bumped. Used by the
   task list (`CoreTaskCreate`, `CoreTaskCreateDefault`, `CoreTaskInsert`). */
s32 CoreListInsert(CoreList *list, void *data, s32 priority)
{
    CoreListNode *node = NULL;
    s32 count;

    if (list->pool != NULL) {
        node = MemPoolAlloc(list->pool);
    }
    if (node == NULL) {
        bool fromLow;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        node = MemAlloc(0x10, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
    }
    if (node == NULL) {
        return -1;
    }
    node->next = NULL;
    node->removed = 0;
    node->data = data;
    node->priority = priority;

    count = list->count;
    if (list->sentinel != NULL) {
        CoreListNode *prev = list->sentinel;
        CoreListNode *cur = prev->next;
        s32 i;

        for (i = 0; i < count; i++) {
            if (cur == NULL || priority < cur->priority) {
                break;
            }
            prev = cur;
            cur = cur->next;
        }
        prev->next = node;
        node->next = cur;
        count = list->count;
    }
    list->count = count + 1;
    return list->count;
}
