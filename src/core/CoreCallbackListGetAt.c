// bdc 0x08a327cc CoreCallbackListGetAt
#include "bdc.h"

/* Returns the payload of the node at `index` of a callback list (`CoreCallbackListNodeAt`), or
   NULL when there is none. Called by `CoreBackgroundProcessRun`. */
void *CoreCallbackListGetAt(CoreList *list, s32 index)
{
    CoreListNode *node = CoreCallbackListNodeAt(list, index);

    if (node == NULL)
        return NULL;
    return node->data;
}
