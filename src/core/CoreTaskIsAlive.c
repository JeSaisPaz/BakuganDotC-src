// bdc 0x089bf980 CoreTaskIsAlive
#include "bdc.h"

/* Returns 1 when `task` is the payload of a `g_taskList` node that is not flagged as removed,
   else 0 (also 0 when the list does not exist). The walk stops at the first node without data. */
int CoreTaskIsAlive(CoreTask *task)
{
    CoreListNode *node;

    if (g_taskList == NULL) {
        return 0;
    }
    for (node = CoreListFirst(g_taskList); node != NULL; node = node->next) {
        if (node->data == NULL) {
            break;
        }
        if (node->data == task && node->removed == 0) {
            return 1;
        }
    }
    return 0;
}
