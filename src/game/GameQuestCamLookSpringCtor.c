// bdc 0x088f8c08 GameQuestCamLookSpringCtor
#include "bdc.h"

/* Constructor of the look-at spring of the quest-field camera system (see `GameQuestCamCtrlCtor`)
   (vtable `g_gameQuestCamLookSpringVtbl`): `GameQuestCamSpringCtor`, look-at `desc+0x30` ->
   `+0x60`, default vector (the `zero` constant of `g_gameQuestCamLookSpringAxisConsts`) ->
   `+0x70`, `+0x80 = 0`, and the descriptor's goal `desc+0x20` -> `+0x40`. The listing copies the
   look-at twice (the second copy is identical). Returns `self`. */

typedef struct CamLookDesc {
  u8 pad[0x20];
  ScePspFVector4 goal;
  ScePspFVector4 look;
} CamLookDesc;

GameQuestCamLookSpring *GameQuestCamLookSpringCtor(GameQuestCamLookSpring *self, void *desc)
{
  CamLookDesc *d = (CamLookDesc *)desc;

  GameQuestCamSpringCtor(&self->base, desc);
  self->base.vtbl = g_gameQuestCamSpringBaseVtbl;
  self->point = d->look;
  self->lookParams = g_gameQuestCamLookSpringAxisConsts.zero;
  self->base.vtbl = g_gameQuestCamLookSpringVtbl;
  self->flag80 = 0;
  self->point = d->look;
  self->base.goal = d->goal;
  return self;
}
