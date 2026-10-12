// bdc 0x088b69b8 BtlItemSystemInit
#include "bdc.h"

/* Initialises the battle-item system: stores the item effect manager `g_btlItemEffectMgr` and
   the object list `g_btlItemObjList` (where `BtlItemInit` puts the item models) and clears
   the list holder `g_btlItemList` (head, tail, count). */

void BtlItemSystemInit(void *effectMgr, void *objList)
{
  g_btlItemEffectMgr = effectMgr;
  g_btlItemList.tail = NULL;
  g_btlItemList.head = NULL;
  g_btlItemList.count = 0;
  g_btlItemObjList = objList;
}

