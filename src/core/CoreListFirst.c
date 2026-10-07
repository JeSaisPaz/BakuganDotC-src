// bdc 0x08a2dc60 CoreListFirst
#include "bdc.h"

/* Returns the first real node of a `CoreList` (the sentinel's `next`), or NULL when the list
   has no sentinel. Used by `CoreTaskFind`-style walks, the task-manager update/draw loops and
   its destructor. */
void *CoreListFirst(void *list)
{
    const CoreList *l = list;

    if (l->sentinel == NULL)
        return NULL;
    return l->sentinel->next;
}
