// bdc 0x089d8dcc CoreNodeGroupAppend
#include "bdc.h"

/* Appends `node` to a `CoreNodeGroup`: when the group is non-empty the node is chained behind
   `group->tail` and `count` is incremented, otherwise it becomes the head and tail with `count =
   1`. Stores `group` in `node->group` (`+0x14`) and clears `node->next`. Returns `node`. */
CoreNode *CoreNodeGroupAppend(CoreNode *node, CoreNodeGroup *group)
{
    if (group->head != NULL) {
        group->tail->next = node;
        node->prev = group->tail;
        group->tail = node;
        group->count++;
    } else {
        group->tail = node;
        group->head = node;
        group->count = 1;
    }
    node->group = group;
    node->next = NULL;
    return node;
}
