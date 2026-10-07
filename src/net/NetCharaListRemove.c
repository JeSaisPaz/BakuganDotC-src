// bdc 0x08a312b4 NetCharaListRemove
#include "bdc.h"

/* Byte-identical compiled copy of `SndRequestListRemove` for `NetCharaList` (same instructions up
   to relocated addresses). */
bool NetCharaListRemove(CoreList *list, void *data)
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
