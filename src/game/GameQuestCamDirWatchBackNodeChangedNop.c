// bdc 0x08a2cc60 GameQuestCamDirWatchBackNodeChangedNop
#include "bdc.h"

/* Empty `node changed` method (vtable entry 3, `vt+0x18`) of the quest camera sub-state WatchBack
   (`{mode, vtable 0x08af6f40}`, `maybe_GameQuestCamModeBaseCtor`): just `jr ra`. */

void GameQuestCamDirWatchBackNodeChangedNop(void *state, s32 node)

{
  return;
}

