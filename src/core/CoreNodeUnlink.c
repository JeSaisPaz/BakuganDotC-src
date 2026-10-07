// bdc 0x089d8cf8 CoreNodeUnlink
#include "bdc.h"

/* Detaches `node` from everything it is linked into: from its owner list (`CoreNodeOwnerRemove`,
   then `owner = NULL`), from its group (`CoreNodeGroupRemove`; the `group` pointer itself is left
   set) and from the sibling chain (`prev->next = next`, `next->prev = prev`), then clears the
   node's own links. Returns the former next sibling, or NULL. */
CoreNode *CoreNodeUnlink(CoreNode *node)
{
    CoreNode *next;

    if (node->owner != NULL) {
        CoreNodeOwnerRemove(node->owner, node);
        node->owner = NULL;
    }
    if (node->group != NULL)
        CoreNodeGroupRemove(node);
    if (node->prev != NULL)
        node->prev->next = node->next;
    next = node->next;
    if (next != NULL)
        next->prev = node->prev;
    node->next = NULL;
    node->prev = NULL;
    return next;
}
