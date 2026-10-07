// bdc 0x088f65b4 GameQuestPathSetUnload
#include "bdc.h"

/* Frees the quest path set singleton `g_questPathSet` (`GameQuestFreePathTable` with flags 3) and
   clears it. */

void GameQuestPathSetUnload(void)

{
  if (g_questPathSet != (void *)0x0) {
    GameQuestFreePathTable(g_questPathSet,3);
  }
  g_questPathSet = (void *)0x0;
  return;
}

