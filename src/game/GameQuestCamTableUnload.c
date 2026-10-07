// bdc 0x088fbec0 GameQuestCamTableUnload
#include "bdc.h"

/* Frees the quest camera table singleton `g_questCamTable` (`GameQuestFreeCamTable` with flags 3) and
   clears the pointer. */

void GameQuestCamTableUnload(void)

{
  if (g_questCamTable != (void *)0x0) {
    GameQuestFreeCamTable(g_questCamTable,3);
  }
  g_questCamTable = (void *)0x0;
  return;
}

