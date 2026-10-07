// bdc 0x08a2cac8 GameQuestCamDirWatchBackExitNop
#include "bdc.h"

/* Empty `exit` method (vtable entry 2, `vt+0x10`) of the quest camera sub-state WatchBack (`{mode,
   vtable 0x08af6f40}`, `maybe_GameQuestCamModeBaseCtor`): just `jr ra`. */

void GameQuestCamDirWatchBackExitNop(void *state)

{
  return;
}

