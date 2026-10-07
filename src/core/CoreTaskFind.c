// bdc 0x089bfa00 CoreTaskFind
#include "bdc.h"

/* Looks up the first task in `g_taskList` whose `id` equals `id` and returns it, or NULL if
   none exists, the list is missing, or that first match is already flagged for removal. A node
   with a NULL task ends the walk. */
void *CoreTaskFind(s32 id)
{
    CoreListNode *node;
    CoreTask *task;

    if (g_taskList == NULL)
        return NULL;
    for (node = CoreListFirst(g_taskList); node != NULL; node = node->next) {
        task = node->data;
        if (task == NULL)
            return NULL;
        if (task->id == id)
            return node->removed == 0 ? task : NULL;
    }
    return NULL;
}
