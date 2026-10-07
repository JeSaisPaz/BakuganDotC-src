// bdc 0x089d86a8 CoreObjectListFindByTag
#include "bdc.h"

/* Returns the first object of a `CoreObject` list holder whose `unk08` tag equals `tag`
   (`CoreObjectFindByTag` on `*list`), or NULL. */
CoreObject *CoreObjectListFindByTag(void *list, u32 tag)
{
    CoreObject **head = list;

    return CoreObjectFindByTag(*head, tag);
}
