// bdc 0x08a29b48 BtlStatsListInsert
#include "bdc.h"

/* Inserts a battle statistics record into a `CoreList` (used by `BtlStatsCreate` on the
   battle stats list): byte-identical compiled copy of `CoreListInsert`. Takes a node from the
   list's `pool` (or 16 bytes from the low heap), stores `data` and `priority`, and links it after
   the last of the first `count` nodes whose priority is `<=` `priority` (equal keys keep insertion
   order). Returns the new `count`, or -1 when no node could be allocated. A list without a
   sentinel still gets its count bumped. */
s32 BtlStatsListInsert(CoreList *list, void *data, s32 priority)
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
