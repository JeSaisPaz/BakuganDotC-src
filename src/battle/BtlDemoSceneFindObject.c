// bdc 0x08904e80 BtlDemoSceneFindObject
#include "bdc.h"

/* Finds the `.scb` scene object with tag `tag` in the scene's object list by forwarding to
   `CoreObjectListFindByTag`. */
CoreObject *BtlDemoSceneFindObject(void **list, u32 tag)
{
    return CoreObjectListFindByTag(list, tag);
}
