// bdc 0x088fda54 GameQuestCamCtrlBaseCtor
#include "bdc.h"

/* Base constructor of the quest-field camera controller (`GameQuestCamCtrlCtor`): installs vtable
   `g_questCamCtrlBaseVtbl` at `+0x2c` and copies the first 0x28 bytes (`eye`, `lookAt`, `extra`) of
   the descriptor `desc`, which shares the controller's layout. Returns `self`. */

GameQuestCamCtrl *GameQuestCamCtrlBaseCtor(GameQuestCamCtrl *self, void *desc)

{
  const GameQuestCamCtrl *src = (const GameQuestCamCtrl *)desc;

  self->vtbl = g_questCamCtrlBaseVtbl;
  self->eye = src->eye;
  self->lookAt = src->lookAt;
  self->extra[0] = src->extra[0];
  self->extra[1] = src->extra[1];
  return self;
}
