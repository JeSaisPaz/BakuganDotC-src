// bdc 0x08a31158 NetCharaListInsert
#include "bdc.h"

/* Inserts `data` into the net-character list `list`, sorted by ascending `priority` (after any
   equal priorities). The 16-byte node comes from the list's pool, else from the low end of the
   heap under the memory lock; returns -1 when neither allocation succeeds, otherwise the new
   element count. Without a sentinel the node is not linked but the count still grows. Called by
   `NetCharaCtor`. */
s32 NetCharaListInsert(CoreList *list, void *data, s32 priority)
{
    bool fromLow;
    CoreListNode *node;
    CoreListNode *prev;
    CoreListNode *cur;
    int count;
    int i;

    node = NULL;
    if (list->pool != NULL)
        node = MemPoolAlloc(list->pool);
    if (node == NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        node = MemAlloc(sizeof(CoreListNode), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
    }
    if (node == NULL)
        return -1;
    node->next = NULL;
    node->removed = 0;

    node->data = data;
    node->priority = priority;
    prev = list->sentinel;
    count = list->count;
    if (prev != NULL) {
        cur = prev->next;
        for (i = 0; i < count; i++) {
            if (cur == NULL || priority < cur->priority)
                break;
            prev = cur;
            cur = cur->next;
        }
        prev->next = node;
        node->next = cur;
        count = list->count;
    }
    count++;
    list->count = count;
    return count;
}
