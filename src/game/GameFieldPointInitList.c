// bdc 0x088d8adc GameFieldPointInitList
#include "bdc.h"

/* Allocates the 0xc-byte field point list holder `g_gameFieldPointList` once and resets the point
   counter `g_gameFieldPointCount`. Caller: `GameFieldPointCreate`. */

void GameFieldPointInitList(void)
{
  bool fromLow;
  GameFieldPoint **list;

  if (g_gameFieldPointList == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = MemAlloc(0xc, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    g_gameFieldPointList = list;
    list[1] = NULL;
    *list = NULL;
    g_gameFieldPointList[2] = NULL;
  }
  g_gameFieldPointCount = 0;
}
