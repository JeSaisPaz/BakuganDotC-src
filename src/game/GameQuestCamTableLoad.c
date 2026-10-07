// bdc 0x088fbb38 GameQuestCamTableLoad
#include "bdc.h"

/* Creates the quest camera table singleton `g_questCamTable` (0x18 bytes, low heap,
   `GameQuestCamTableCtor`) from the loaded `.cptb` data `src` unless it already exists. */

void GameQuestCamTableLoad(void *src)

{
  bool fromLow;
  GameQuestCamTable *table;
  GameQuestCamTable *result;

  result = g_questCamTable;
  if (g_questCamTable == (GameQuestCamTable *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    table = (GameQuestCamTable *)MemAlloc(0x18,(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    result = (GameQuestCamTable *)0x0;
    if (table != (GameQuestCamTable *)0x0) {
      GameQuestCamTableCtor(table,(void **)src);
      result = table;
    }
  }
  g_questCamTable = result;
  return;
}
