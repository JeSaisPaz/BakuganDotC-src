// bdc 0x088fc2fc GameQuestCamSpringCtor
#include "bdc.h"

/* Constructor of the spring-driven point used by the quest camera modes (vtable
   `g_gameQuestCamSpringVtbl`): stores the owner pair `desc->ctrl`/`desc->followed`, sets the
   stiffness to 10.0, copies `desc->start` into `vel` and `desc->goal` into `goal`, and clears
   `accel`/`step` from the zero vector of `g_gameQuestCamSpringAxisConsts`. Returns `self`. */

GameQuestCamSpring *GameQuestCamSpringCtor(GameQuestCamSpring *self, void *desc)
{
  GameQuestCamSpringDesc *d = (GameQuestCamSpringDesc *)desc;

  self->vtbl = g_gameQuestCamSpringVtbl;
  self->ctrl = d->ctrl;
  self->followed = d->followed;
  self->stiffness = 10.0f;
  self->vel = d->start;
  self->accel = g_gameQuestCamSpringAxisConsts.zero;
  self->goal = d->goal;
  self->step = g_gameQuestCamSpringAxisConsts.zero;
  return self;
}
