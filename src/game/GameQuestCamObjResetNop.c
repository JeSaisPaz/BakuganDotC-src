// bdc 0x08a2c784 GameQuestCamObjResetNop
#include "bdc.h"

/* Entry 4 (reset, `+0x24`) of the quest camera root vtable `0x08af6ee0` (`GameQuestCamObjDtor`):
   empty; overridden in the camera modes by `GameQuestCamModeResetSmooth`. */

void GameQuestCamObjResetNop(void *obj)

{
  return;
}

