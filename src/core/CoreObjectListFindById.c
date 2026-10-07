// bdc 0x089d868c CoreObjectListFindById
#include "bdc.h"

/* Looks up a `CoreObject` by id in a chain given by its head holder: `head` points to a struct
   whose first word is the first object (e.g. `g_btlBakuganList`); returns
   `CoreObjectFindById``(*head, id)`. */
CoreObject *CoreObjectListFindById(CoreObject **head, u32 id)
{
    return CoreObjectFindById(*head, id);
}
