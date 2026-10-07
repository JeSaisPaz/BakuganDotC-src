// bdc 0x089d9144 CoreNodeOwnerAppend
#include "bdc.h"

/* Appends `node` to the owner's list: sets `node->owner`, makes it the head if the list was empty,
   otherwise splices it behind the current tail with `CoreNodeSpliceChain`; `tail` always becomes
   `node`. A NULL `node` is ignored. */
void CoreNodeOwnerAppend(CoreNodeOwner *owner, CoreNode *node)
{
    if (node == NULL) {
        return;
    }
    node->owner = owner;
    if (owner->tail != NULL) {
        CoreNodeSpliceChain(node, owner->tail);
        owner->tail = node;
    } else {
        owner->tail = node;
        if (owner->head == NULL) {
            owner->head = node;
        }
    }
}
