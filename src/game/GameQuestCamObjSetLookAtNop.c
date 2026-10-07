// bdc 0x08a2c77c GameQuestCamObjSetLookAtNop
#include "bdc.h"

/* Entry 3 (`+0x1c`) of the quest camera root vtable `0x08af6ee0` (`GameQuestCamObjDtor`): empty;
   inherited by the spring point/camera modes and the eye and rail springs, overridden by
   `GameQuestCamLookSpringSetLookAt`. */

void GameQuestCamObjSetLookAtNop(void *obj, const float *v)

{
  return;
}

