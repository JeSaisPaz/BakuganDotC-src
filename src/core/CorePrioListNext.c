// bdc 0x08a306bc CorePrioListNext
#include "bdc.h"

/* Cursor walk: returns the payload of the next non-removed node from the cursor onward and
   advances the cursor past it, or NULL at the end. A non-removed node with a NULL payload is
   stepped over like a removed one. */
void *CorePrioListNext(CorePrioList *list)
{
    void *data = NULL;

    while (list->cursor != NULL) {
        if (!CorePrioNodeIsRemoved(list->cursor))
            data = CorePrioNodeGetData(list->cursor);
        list->cursor = CorePrioNodeGetNext(list->cursor);
        if (data != NULL)
            break;
    }
    return data;
}
