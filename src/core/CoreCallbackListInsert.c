// bdc 0x08a2d308 CoreCallbackListInsert
#include "bdc.h"

/* Adds `payload` (here a callback function pointer) to a `CoreCallbackListInit` list with the
   given `priority`, keeping the list sorted by ascending priority: a new node goes in front of the
   first node whose priority is strictly greater, so equal priorities stay in insertion order (the
   walk stops after `count` nodes). The node comes from the list's node pool (`MemPoolAlloc`) or,
   when the pool is exhausted or absent, from the low end of the heap. Returns the new element
   count, or -1 if no node could be allocated. A list without a sentinel only gets its count
   bumped. */
int CoreCallbackListInsert(CoreList *list, void *payload, s32 priority)
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

    node->data = payload;
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
