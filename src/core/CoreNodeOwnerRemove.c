// bdc 0x089d91a4 CoreNodeOwnerRemove
#include "bdc.h"

/* Fixes the owner's `head`/`tail` for a node that is leaving the list (head becomes the node's next
   sibling, tail its previous one) and returns the node's former next sibling, or NULL for a NULL
   node. The sibling links themselves are repaired by `CoreNodeUnlink`. */
CoreNode *CoreNodeOwnerRemove(CoreNodeOwner *owner, CoreNode *node)
{
    CoreNode *next;

    if (node == NULL) {
        return NULL;
    }
    next = node->next;
    if (node == owner->head) {
        owner->head = next;
    }
    if (node == owner->tail) {
        owner->tail = node->prev;
    }
    return next;
}
