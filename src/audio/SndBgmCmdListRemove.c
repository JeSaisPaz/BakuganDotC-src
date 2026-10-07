// bdc 0x08a2fbb8 SndBgmCmdListRemove
#include "bdc.h"

/* Removes the first node whose payload is `data` from the BGM command list. When the list is not
   being walked (`iterating == 0`) the node is unlinked and returns 1; if it was not already flagged
   removed it is also returned to the pool (`MemPoolFree`) or freed on the heap and `count` is
   decremented. While a walk is running the node is only flagged removed (`removed = 1`) and kept:
   returns 1, or 0 if it was already flagged. Returns 0 if no node holds `data`. Called by
   `SndBgmCmdDestroy`. */
bool SndBgmCmdListRemove(CoreList *list, SndBgmCmd *data)
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
