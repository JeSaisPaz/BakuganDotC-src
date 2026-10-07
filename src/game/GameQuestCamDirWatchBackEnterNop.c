// bdc 0x08a2cac0 GameQuestCamDirWatchBackEnterNop
#include "bdc.h"

/* Empty `enter` method (vtable entry 1, `vt+0x8`) of the quest camera sub-state WatchBack (`{mode,
   vtable 0x08af6f40}`, `maybe_GameQuestCamModeBaseCtor`): just `jr ra`. */

void GameQuestCamDirWatchBackEnterNop(void *state, void *arg)

{
  return;
}

