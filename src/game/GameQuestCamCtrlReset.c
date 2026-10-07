// bdc 0x088fa464 GameQuestCamCtrlReset
#include "bdc.h"

/* Resets the quest-field camera controller (field camera `+0x5c4`, see `GameFieldCameraReset`;
   tables from `GameQuestParsePathTable`/`GameQuestParseCamTable`): calls vtable slot `+0x24`
   (reset) of both owned camera-mode objects `+0x3c` and `+0x38`. */

void GameQuestCamCtrlReset(GameQuestCamCtrl *self)

{
  GameQuestCamSpring *spring;
  
  spring = self->look;
  if (spring == (GameQuestCamSpring *)0x0) {
    spring = self->mode;
  }
  else {
    ((void (*)(void *))spring->vtbl[4].fn)((char *)spring + spring->vtbl[4].delta);
    spring = self->mode;
  }
  if (spring != (GameQuestCamSpring *)0x0) {
    ((void (*)(void *))spring->vtbl[4].fn)((char *)spring + spring->vtbl[4].delta);
  }
  return;
}

