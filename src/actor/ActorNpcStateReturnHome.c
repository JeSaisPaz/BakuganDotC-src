// bdc 0x088e7c1c ActorNpcStateReturnHome
#include "bdc.h"

/* AI state 10 of the field NPC/guard classes (base `ActorNpcCtor`) (slot 45): unpauses the field,
   walks back to the remembered point `returnPoint` (virtual slot 18; 1.5x speed, radius 6 and run on
   stages 0x14 and 5, else radius 2), turns to the home heading `homeHeading` (turning motions 9/10
   for models 0x4e..0x50), then returns to state 0 and calls slot 46 with 1. */

void ActorNpcStateReturnHome(ActorNpc *self)
{
  const VtblEntry *entry;
  s32 (*walk)(void *, float *, s32, s32, float, float);
  s32 stage;
  s32 slot;
  float turn;

  if (self->subStep < 1) {
    if (self->subStep < 0) {
      return;
    }
    if (g_gameFieldCharSet->paused != 0) {
      g_gameFieldCharSet->paused = 0;
    }
    entry = (const VtblEntry *)self->base.base.base.vtable + 18;
    walk = (s32 (*)(void *, float *, s32, s32, float, float))entry->fn;
    stage = g_scriptGlobalVars[1];
    if (stage == 0x14 || stage == 5) {
      if (walk((char *)self + entry->delta, self->returnPoint, 0, 1, self->speed * 1.5f, 6.0f) != 0) {
        self->subStep = 1;
      }
    } else {
      if (walk((char *)self + entry->delta, self->returnPoint, 0, 0, self->speed, 2.0f) != 0) {
        self->subStep = 1;
      }
    }
  } else if (self->subStep < 2) {
    turn = ActorTurnToward(self->homeHeading, 1.0f, 0.034906585f, self);
    if (turn * turn < 0.01f) {
      self->subStep = 2;
    }
    if ((s32)self->base.base.base.unk08 >= 0x4e && (s32)self->base.base.base.unk08 < 0x51) {
      slot = 9;
      if (turn < 0.0f) {
        slot = 10;
      }
      ActorPlayMotion(0.2f, self, slot, 1, 0);
    }
  } else if (self->subStep < 3) {
    entry = (const VtblEntry *)self->base.base.base.vtable + 46;
    self->aiState = 0;
    self->subStep = 0;
    ((void (*)(void *, s32))entry->fn)((char *)self + entry->delta, 1);
  }
}
