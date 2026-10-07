// bdc 0x088d844c GameGimmickCameraSetConeVisible
#include "bdc.h"

/* Sets the view-cone visibility byte `+0x254` of the surveillance camera gimmick
   (`GameGimmickCameraCtor`, vtables `0x08af325c`/`0x08af3304`). Caller: `GameFieldSetGimmicks0bd9A` (event
   command). */

void GameGimmickCameraSetConeVisible(GameGimmickCamera *obj, u8 visible)

{
  obj->coneVisible = visible;
  return;
}

