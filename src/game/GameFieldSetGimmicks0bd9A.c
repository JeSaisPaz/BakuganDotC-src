// bdc 0x088ea00c GameFieldSetGimmicks0bd9A
#include "bdc.h"

/* Calls `GameGimmickCameraSetConeVisible(gimmick, value)` on every gimmick of type id 0xbd9 in the
   field's gimmick list (`GameFieldFindTask()+0x658`). */

void GameFieldSetGimmicks0bd9A(void *blind, u8 value)
{
  GameGimmick *obj;

  for (obj = ((GameFieldTask *)GameFieldFindTask())->gimmicks; obj != NULL; obj = (GameGimmick *)obj->base.base.next) {
    if (obj->typeId == 0xbd9) {
      GameGimmickCameraSetConeVisible((GameGimmickCamera *)obj, value);
    }
  }
}
