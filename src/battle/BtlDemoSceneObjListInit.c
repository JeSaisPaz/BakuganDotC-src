// bdc 0x08904c7c BtlDemoSceneObjListInit
#include "bdc.h"

/* Clears a scene object list head `{head, tail, count}` (scene `+0x1a0`). Returns `list`. */

BtlDemoSceneList *BtlDemoSceneObjListInit(BtlDemoSceneList *list)
{
    list->tail = NULL;
    list->head = NULL;
    list->count = 0;
    return list;
}
