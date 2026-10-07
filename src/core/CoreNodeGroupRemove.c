// bdc 0x089d8e40 CoreNodeGroupRemove
#include "bdc.h"

/* Removes `node` from the `CoreNodeGroup` in `node->group`: repairs `head` (when the node was the
   head, the next sibling takes over) and `tail` (when the node was the tail it is replaced by a
   neighbour, falling back to the last node of the head chain) and decrements `count`. Does not
   touch the node's own links. */
void CoreNodeGroupRemove(CoreNode *node)
{
    CoreNodeGroup *group = node->group;
    CoreNode *last;

    if (group->tail == node) {
        if (node->next != NULL) {
            group->tail = node->next;
        } else if (node->prev != NULL) {
            group->tail = node->prev;
        } else {
            last = group->head;
            while (last->next != NULL) {
                last = last->next;
            }
            group->tail = last;
        }
        group = node->group;
    }
    if (group->head == node) {
        group->head = node->next;
        group = node->group;
    }
    group->count--;
}
