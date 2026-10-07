// bdc 0x088f60fc GameQuestPathSetLoad
#include "bdc.h"

/* Creates the quest path set singleton `g_questPathSet` from the loaded `.ptlb` file on first use
   (`GameQuestPathSetCtor`): allocates it from the low heap under the heap lock. Does nothing when
   the singleton already exists; stores NULL when the allocation fails. Called by
   `GameFieldCameraLoadQuestCam`. */

void GameQuestPathSetLoad(void **desc)
{
  bool fromLow;
  GameQuestPathSet *set;

  if (g_questPathSet == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    set = (GameQuestPathSet *)MemAlloc(sizeof(GameQuestPathSet), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (set != NULL) {
      GameQuestPathSetCtor(set, desc);
    }
    g_questPathSet = set;
  }
}
