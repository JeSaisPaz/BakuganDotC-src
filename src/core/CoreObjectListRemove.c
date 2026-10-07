// bdc 0x089d8a84 CoreObjectListRemove
#include "bdc.h"

/* Removes `obj` from its owner list holder (`obj->list`, a `CoreObjectList`): when it is the
   tail, the tail moves to its `next`, its `prev`, or the last node found by walking from the head;
   when it is the head, the head becomes its `next`; the count is decremented. The node's own links
   are fixed by the caller `CoreObjectUnlink`. */
void CoreObjectListRemove(CoreObject *obj)
{
    CoreObjectList *list = obj->list;
    CoreObject *last;

    if (list->tail == obj) {
        if (obj->next != NULL) {
            list->tail = obj->next;
        } else if (obj->prev != NULL) {
            list->tail = obj->prev;
        } else {
            last = list->head;
            if (last != NULL) {
                while (last->next != NULL) {
                    last = last->next;
                }
            }
            list->tail = last;
        }
        list = obj->list;
    }
    if (list->head == obj) {
        list->head = obj->next;
        list = obj->list;
    }
    list->count--;
}
