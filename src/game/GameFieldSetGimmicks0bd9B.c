// bdc 0x088ea070 GameFieldSetGimmicks0bd9B
#include "bdc.h"

/* Calls `GameGimmickCameraSetSpotVisible(gimmick, value)` on every gimmick of type id 0xbd9 in the field's gimmick
   list. */

void GameFieldSetGimmicks0bd9B(void *blind, u8 value)

{
  u16 typeId;
  GameFieldTask *task;
  GameGimmickCamera *obj;

  task = (GameFieldTask *)GameFieldFindTask();
  obj = (GameGimmickCamera *)task->gimmicks;
  if (obj != (GameGimmickCamera *)0x0) {
    typeId = (obj->base).typeId;
    while( true ) {
      if (typeId == 0xbd9) {
        GameGimmickCameraSetSpotVisible(obj,value);
      }
      obj = (GameGimmickCamera *)(obj->base).base.base.next;
      if (obj == (GameGimmickCamera *)0x0) break;
      typeId = (obj->base).typeId;
    }
  }
  return;
}

