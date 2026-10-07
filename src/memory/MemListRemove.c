// bdc 0x089d7aa8 MemListRemove
#include "bdc.h"

/* Unlinks `blk` from the doubly-linked `MemBlockList` `list`, moving the list cursor to a
   neighbour (`next`, else `prev`; cleared when that neighbour is the list sentinel itself) if it
   pointed at `blk`, and clears `blk->prev/next`. Returns false only for a NULL block. */
bool MemListRemove(MemBlockList *list, MemBlock *blk)
{
    if (blk == NULL) {
        return false;
    }
    if (blk->prev != NULL) {
        blk->prev->next = blk->next;
    }
    if (blk->next != NULL) {
        blk->next->prev = blk->prev;
    }
    if (list->cursor == blk) {
        MemBlock *cursor = blk->next;

        if (cursor == NULL) {
            cursor = blk->prev;
        }
        list->cursor = cursor;
        /* The first block's prev is the sentinel, i.e. the list header itself. */
        if ((void *)cursor == (void *)list) {
            list->cursor = NULL;
        }
    }
    blk->prev = NULL;
    blk->next = NULL;
    return true;
}
