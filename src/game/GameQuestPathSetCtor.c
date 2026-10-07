// bdc 0x088f6064 GameQuestPathSetCtor
#include "bdc.h"

/* Constructor of the quest path set (`{data, capacity, count, flag}`): allocates a 10-entry pointer
   vector (low heap) and parses the room's path table `desc->data`/`desc->size` into it
   (`GameQuestParsePathTable`). */

void *GameQuestPathSetCtor(void *setPtr, void **desc)

{
  bool fromLow;
  GameQuestPathSet *set = (GameQuestPathSet *)setPtr;
  
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  set->data = (void **)MemAlloc(0x28,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  set->cap = 10;
  set->count = 0;
  set->flag = 0;
  GameQuestParsePathTable(set,((GameQuestPathFileDesc *)desc)->data,((GameQuestPathFileDesc *)desc)->size);
  return setPtr;
}

