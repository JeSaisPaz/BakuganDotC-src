// bdc 0x08908534 BtlDemoSceneEventListInit
#include "bdc.h"

/* Clears the scene's event table list head `{head, tail, count}` (scene `+0x1ac`). Returns `list`. */
BtlDemoSceneList *BtlDemoSceneEventListInit(BtlDemoSceneList *list)
{
    list->tail = NULL;
    list->head = NULL;
    list->count = 0;
    return list;
}
