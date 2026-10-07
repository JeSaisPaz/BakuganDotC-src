// bdc 0x089d8840 CoreObjectUnlink
#include "bdc.h"

/* Unlinks a `CoreObject` from its sibling chain and, when it belongs to a `CoreObjectList`
   (`list` set), from that list first (`CoreObjectListRemove`). Afterwards `obj->prev` and
   `obj->next` are cleared. Returns the former next object (NULL if it was last). Used by the
   `CoreObject` destructor and `CoreObjectDeferDelete`. */
CoreObject *CoreObjectUnlink(CoreObject *obj)
{
    CoreObject *next;

    if (obj->list != NULL) {
        CoreObjectListRemove(obj);
    }
    if (obj->prev != NULL) {
        obj->prev->next = obj->next;
    }
    next = obj->next;
    if (next != NULL) {
        next->prev = obj->prev;
    }
    obj->next = NULL;
    obj->prev = NULL;
    return next;
}
