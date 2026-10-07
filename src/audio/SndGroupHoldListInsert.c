// bdc 0x08a2f01c SndGroupHoldListInsert
#include "bdc.h"

/* Inserts `data` into the group hold list of the `SndGroupLoader` (`holds`, data = 8-byte
   `{groupId, ttl}` records) (a compiler copy of the engine's priority-sorted singly linked list,
   same code as `CoreListInit`'s list): takes a 0x10-byte node from the list's `MemPool`
   (`list+0xc`) or, failing that, the low heap, fills it (`+4 priority`, `+0xc data`, `removed = 0`)
   and links it in front of the first node with a strictly greater `priority` (after equal ones),
   scanning at most `count` nodes from the sentinel `list+8`. Increments `count` and returns it, or
   -1 if no node could be allocated. */
s32 SndGroupHoldListInsert(CoreList *list, void *data, s32 priority)
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
