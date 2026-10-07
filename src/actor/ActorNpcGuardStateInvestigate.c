// bdc 0x088e8eb4 ActorNpcGuardStateInvestigate
#include "bdc.h"

/* AI state 5 (slot 40) of the guard classes (`ActorNpcGuardCtor`, vtable `0x08af4024`; inherited
   by the cloaked guard `ActorNpcCloakCtor`), overriding `ActorNpcStateInvestigateBase`: shows
   the alert head effect 0x29 (`ActorNpcShowHeadEffect`), calls slot 46
   (`ActorNpcGuardSetAlertView`) with 0 and sets `lookMode = 1`, waits 30 frames, turns toward
   `noisePoint` (motion 8), looks with motion 0xb for 60 frames, idles 30 frames while the mark
   blinks (`ActorNpcSetEffectAlpha`), walks back to `returnPoint` (slot 18), turns to
   `homeHeading` and returns to state 0 (slot 46 with 1, `lookMode = 0`). Does nothing when
   `ActorNpcCheckInterrupt` took over. */

void ActorNpcGuardStateInvestigate(ActorNpcGuard *self)
{
  ActorNpc *npc = &self->base;
  const VtblEntry *entry;
  float turn;
  s32 done;

  if (ActorNpcCheckInterrupt(npc, 0) != 0) {
    return;
  }
  switch (npc->subStep) {
  case 0:
    ActorNpcShowHeadEffect(npc, 0x29, 1, 1);
    entry = (const VtblEntry *)npc->base.base.base.vtable + 46;
    npc->subStep = 1;
    ((void (*)(void *, s32))entry->fn)((char *)self + entry->delta, 0);
    self->lookMode = 1;
    npc->timer = 30;
    break;
  case 1:
    if (npc->timer == 0) {
      npc->subStep = 2;
    } else {
      npc->timer = npc->timer - 1;
    }
    break;
  case 2:
    turn = atan2f(npc->noisePoint[2] - npc->base.base.pos[2], npc->noisePoint[0] - npc->base.base.pos[0]);
    turn = ActorTurnToward(turn, 1.0f, 0.06981317f, self);
    if (turn * turn < 0.01f) {
      npc->subStep = 3;
      ActorPlayMotion(0.2f, self, 0xb, 0, 0);
      npc->timer = 60;
    } else {
      ActorPlayMotion(0.2f, self, 8, 0, 0);
    }
    break;
  case 3:
    if (npc->timer == 0) {
      done = 1;
    } else {
      npc->timer = npc->timer - 1;
      done = 0;
    }
    if (done) {
      npc->subStep = 5;
      ActorPlayMotion(0.2f, self, 0, 1, 0);
      npc->timer = 30;
    }
    break;
  case 5:
    if (npc->timer == 0) {
      npc->subStep = 6;
      ActorNpcShowHeadEffect(npc, 0x29, 0, 1);
    } else {
      npc->timer = npc->timer - 1;
      ActorNpcSetEffectAlpha((float)(npc->timer % 2), npc, 0x29);
    }
    break;
  case 6:
    entry = (const VtblEntry *)npc->base.base.base.vtable + 18;
    if (((s32 (*)(void *, float *, s32, s32, float, float))entry->fn)(
            (char *)self + entry->delta, npc->returnPoint, 0, 0, 1.0f, 3.0f) != 0) {
      npc->subStep = 7;
    }
    break;
  case 7:
    turn = ActorTurnToward(npc->homeHeading, 1.0f, 0.06981317f, self);
    if (turn * turn < 0.01f) {
      npc->subStep = 8;
    }
    break;
  case 8:
    npc->aiState = 0;
    entry = (const VtblEntry *)npc->base.base.base.vtable + 46;
    npc->subStep = 0;
    npc->forceUpdate = 0;
    ((void (*)(void *, s32))entry->fn)((char *)self + entry->delta, 1);
    self->lookMode = 0;
    break;
  default:
    break;
  }
}
