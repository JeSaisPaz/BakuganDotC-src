// bdc 0x088d8458 GameGimmickCameraSetSpotVisible
#include "bdc.h"

/* Sets the spot visibility byte `+0x255` of the surveillance camera gimmick
   (`GameGimmickCameraCtor`, vtables `0x08af325c`/`0x08af3304`). Caller: `GameFieldSetGimmicks0bd9B` (event
   command). */

void GameGimmickCameraSetSpotVisible(GameGimmickCamera *obj, u8 visible)

{
  obj->spotVisible = visible;
  return;
}

