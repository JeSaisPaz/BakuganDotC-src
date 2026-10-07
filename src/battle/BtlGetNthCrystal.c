// bdc 0x08854fc8 BtlGetNthCrystal
#include "bdc.h"

/* Returns the `index`-th crystal (`ActorCrystalCtor`) of `g_btlBakuganList` in list order:
   walks the chain, buffers the id of every node whose virtual predicate (vtable entry 11, the
   crystal test) returns non-zero, and when `index` is below the count resolves that id with
   `CoreObjectListFindById`. Returns NULL without a list or with too few crystals. The buffer
   holds 21 ids and is not bounds-checked. */
void *BtlGetNthCrystal(int index)
{
    CoreObject **head;
    CoreObject *node;
    int count;
    u32 ids[21];

    head = BtlGetBakuganList();
    if (head == NULL) {
        return NULL;
    }
    node = *head;
    count = 0;
    if (node == NULL) {
        return NULL;
    }
    do {
        const VtblEntry *isCrystal = &((const VtblEntry *)node->vtable)[11];

        if (((int (*)(void *))isCrystal->fn)((u8 *)node + isCrystal->delta) != 0) {
            ids[count++] = node->id;
        }
        node = node->next;
    } while (node != NULL);
    if (index < count) {
        return CoreObjectListFindById(head, ids[index]);
    }
    return NULL;
}
