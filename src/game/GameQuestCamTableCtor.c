// bdc 0x088fba9c GameQuestCamTableCtor
#include "bdc.h"

/* Constructor of the quest camera table: allocates a 10-entry pointer vector from the low heap
   (`{data, cap=10, count=0}`, flags `+0x10`/`+0x11` cleared) and fills it from the `.cptb` file
   `*src` with `GameQuestParseCamTable`. Returns `table`. */

void *GameQuestCamTableCtor(void *tablePtr, void **src)

{
  bool fromLow;
  GameQuestCamTable *table = (GameQuestCamTable *)tablePtr;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  table->data = (struct GameQuestCamEntry **)MemAlloc(10 * sizeof(struct GameQuestCamEntry *),(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  table->cap = 10;
  table->count = 0;
  table->flag0 = 0;
  table->flag1 = 0;
  GameQuestParseCamTable(table,*src);
  return tablePtr;
}
