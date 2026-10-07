// bdc 0x08a3276c CoreCallbackListNodeAt
#include "bdc.h"

/* Returns the `index`-th node of a callback list (0 = first, `CoreCallbackListFirst`), or NULL
   when `index` is negative, not below `list->count`, or the chain ends early. Byte-identical
   compiled copy of `SndBgmCmdListNodeAt`. */
CoreListNode *CoreCallbackListNodeAt(CoreList *list, s32 index)
{
    CoreListNode *node;

    if (index < 0 || index >= list->count) {
        return NULL;
    }
    node = CoreCallbackListFirst(list);
    if (node == NULL) {
        return NULL;
    }
    while (--index >= 0) {
        if (node == NULL) {
            return NULL;
        }
        node = node->next;
    }
    return node;
}
