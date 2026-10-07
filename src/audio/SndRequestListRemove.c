// bdc 0x08a2ecd8 SndRequestListRemove
#include "bdc.h"

/* Removes the first node of the outstanding sound-request list of the `SndGroupLoader`
   (`requests`, data = 12-byte `{soundId, arg, u8 released, u8 touched}` records) whose `data`
   equals the argument. While the list is not being iterated (`iterating == 0`) the node is
   unlinked at once and returns 1; if it was not already flagged `removed` it is also returned to
   the list's pool (or the heap when there is no pool or `MemPoolFree` fails) and `count` is
   decremented, while an already-flagged node is only unlinked. During iteration the node is only
   flagged `removed = 1` and left for the purge (returns 1); if it was already flagged it returns
   0. Returns 0 when no node matches or the list has no sentinel. */
bool SndRequestListRemove(CoreList *list, void *data)
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
