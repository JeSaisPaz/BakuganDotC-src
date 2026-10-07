// bdc 0x089bf8fc CoreTaskExists
#include "bdc.h"

/* Returns 1 if the global task list `g_taskList` contains at least one live task whose `id`
   equals `id`, else 0 (also 0 when the list does not exist). Nodes flagged as pending removal are
   ignored; the walk stops at the first node without data and does not stop early on a match. */
s32 CoreTaskExists(s32 id)
{
    CoreListNode *node;
    CoreTask *task;
    s32 found;

    found = 0;
    if (g_taskList == NULL) {
        return 0;
    }
    for (node = CoreListFirst(g_taskList); node != NULL; node = node->next) {
        task = node->data;
        if (task == NULL) {
            break;
        }
        if (task->id == id && node->removed == 0) {
            found = 1;
        }
    }
    return found;
}
