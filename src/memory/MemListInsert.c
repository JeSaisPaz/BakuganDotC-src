// bdc 0x089d840c MemListInsert
#include "bdc.h"

/* Links `blk` right after `pos`. */
static void MemListLinkAfter(MemBlock *pos, MemBlock *blk)
{
    blk->prev = pos;
    blk->next = pos->next;
    pos->next->prev = blk;
    pos->next = blk;
}

/* Links `blk` right before `pos`. */
static void MemListLinkBefore(MemBlock *pos, MemBlock *blk)
{
    blk->next = pos;
    blk->prev = pos->prev;
    pos->prev->next = blk;
    pos->prev = blk;
}

/* Inserts `blk` into `MemBlockList` `list` ordered by `blk->data` (address), starting the search
   at the list cursor (or head) and leaving the cursor on `blk`. With `sorted` the walk continues to
   the exact position; without it only one step is taken from the cursor. An empty list gets `blk`
   as head with the list header as its `prev` (sentinel). A block appended after the last block
   keeps its own `next`. */
void MemListInsert(MemBlockList *list, MemBlock *blk, bool sorted)
{
    MemBlock *cur;
    MemBlock *step;
    void *data;

    if (blk == NULL) {
        return;
    }
    cur = list->cursor;
    if (cur == NULL) {
        cur = list->head;
    }
    if (list->head == NULL) {
        list->head = blk;
        blk->prev = (MemBlock *)list;
        list->cursor = blk;
        return;
    }

    data = blk->data;
    if (data < cur->data) {
        /* Walk backwards. */
        if (cur == NULL) {
            list->cursor = blk;
            return;
        }
        do {
            if (cur->data < data) {
                MemListLinkAfter(cur, blk);
                goto done;
            }
            step = cur->prev;
            if (step == NULL) {
                MemListLinkAfter(cur, blk);
                goto done;
            }
            if (step->next != cur) {
                MemListLinkBefore(cur, blk);
                goto done;
            }
            cur = step;
        } while (sorted);
        MemListLinkAfter(cur, blk);
    } else {
        /* Walk forwards. */
        if (cur == NULL) {
            list->cursor = blk;
            return;
        }
        do {
            if (data < cur->data) {
                MemListLinkBefore(cur, blk);
                goto done;
            }
            step = cur->next;
            if (step == NULL) {
                cur->next = blk;
                blk->prev = cur;
                goto done;
            }
            if (step->prev != cur) {
                MemListLinkAfter(cur, blk);
                goto done;
            }
            cur = step;
        } while (sorted);
        MemListLinkBefore(cur, blk);
    }
done:
    list->cursor = blk;
}
