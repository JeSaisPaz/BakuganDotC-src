// bdc 0x089d8954 CoreObjectListAppend
#include "bdc.h"

/* Appends `obj` to the tail of the `CoreObjectList` `list`: an empty list gets
   `head = tail = obj`, otherwise the old tail's `next` and `obj->prev` are linked; the count is
   incremented and `obj->list` is set to the owning list. Returns `obj`; a NULL `list` does
   nothing. */
CoreObject *CoreObjectListAppend(CoreObject *obj, CoreObjectList *list)
{
    if (list == NULL) {
        return obj;
    }
    if (list->head == NULL) {
        list->tail = obj;
        list->head = obj;
        list->count = 1;
    } else {
        list->tail->next = obj;
        obj->prev = list->tail;
        list->tail = obj;
        list->count++;
    }
    obj->list = list;
    return obj;
}
