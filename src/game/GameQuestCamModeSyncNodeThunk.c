// bdc 0x088fd490 GameQuestCamModeSyncNodeThunk
#include "bdc.h"

/* Thunk to `GameQuestCamModeSyncNode`, called from the mode updates `GameQuestCamPathModeUpdateTarget` and
   `GameQuestCamPointModeUpdateTarget`. */

void GameQuestCamModeSyncNodeThunk(GameQuestCamModeBase *self)

{
  GameQuestCamModeSyncNode(self);
  return;
}

