// bdc 0x08a2f178 SndGroupHoldListRemove
#include "bdc.h"

/* Removes the first node of the group hold list of the `SndGroupLoader` (`holds`, data = 8-byte
   `{groupId, ttl}` records) whose `data` equals the argument. While the list is not being iterated
   (`list+0 == 0`) the node is unlinked at once; during iteration it is only flagged `removed = 1`
   and left for the purge. Returns 1 if a node was found (and unlinked or flagged), else 0. A node
   unlinked at once is returned to the list's pool (or heap) and `count` is decremented. */
bool SndGroupHoldListRemove(CoreList *list, void *data)
{
    CoreListNode *prev = list->sentinel;
    CoreListNode *node;

    if (prev == NULL) {
        return false;
    }
    for (node = prev->next; node != NULL; prev = node, node = node->next) {
        if (node->data != data) {
            continue;
        }
        if (list->iterating) {
            if (node->removed) {
                return false;
            }
            node->removed = 1;
            return true;
        }
        prev->next = node->next;
        node->next = NULL;
        if (node->removed) {
            return true;
        }
        if (list->pool == NULL || !MemPoolFree(list->pool, node)) {
            MemLock();
            MemFree(node, NULL, 0);
            MemUnlock();
        }
        list->count--;
        return true;
    }
    return false;
}
