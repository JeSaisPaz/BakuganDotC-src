// bdc 0x088e954c ActorNpcGuardStateChaseBall
#include "bdc.h"

/* AI state 6 (slot 41) of the guard classes (`ActorNpcGuardCtor`, vtable `0x08af4024`; inherited
   by `ActorNpcCloakCtor`), overriding `ActorNpcState06Nop`: shows head effect 0x29
   (`ActorNpcShowHeadEffect`), hides the view cone, remembers its own position in `returnPoint`
   and the position of the player's ball (`ActorPlayer.ball`, `ActorBall.vec160`) in `ballPos`,
   waits 30 frames, turns toward the ball (motion 8, or 0xb once facing it), walks there (slot 18,
   speed 0.5, arrive distance 12), plays motion 0xd twice, turns and walks back to `returnPoint`,
   turns to `homeHeading`, blinks the mark for 30 frames (`ActorNpcSetEffectAlpha`) and returns
   to state 0 with the effect cleared and the view cone shown again. Does nothing when
   `ActorNpcCheckInterrupt` took over. */

void ActorNpcGuardStateChaseBall(ActorNpcGuard *self)
{
  ActorNpc *npc = &self->base;
  ActorPlayer *player;
  ActorBall *ball;
  const VtblEntry *entry;
  float target[4];
  float turn;
  s32 timer;

  if (ActorNpcCheckInterrupt(npc, 0) != 0) {
    return;
  }
  switch (npc->subStep) {
  case 0:
    player = (ActorPlayer *)ActorFindPlayer();
    ActorNpcShowHeadEffect(npc, 0x29, 1, 1);
    npc->subStep = 1;
    npc->timer = 30;
    npc->returnPoint[0] = npc->base.base.pos[0];
    npc->returnPoint[1] = npc->base.base.pos[1];
    npc->returnPoint[2] = npc->base.base.pos[2];
    npc->returnPoint[3] = npc->base.base.pos[3];
    ((ActorNpcViewCone *)npc->viewCone)->visible = 0;
    /* the asm first stores the stale VFPU register C720 into ballPos; it is overwritten here */
    ball = (ActorBall *)player->ball;
    target[0] = ball->vec160[0];
    target[1] = ball->vec160[1];
    target[2] = ball->vec160[2];
    target[3] = ball->vec160[3];
    self->ballPos[0] = target[0];
    self->ballPos[1] = target[1];
    self->ballPos[2] = target[2];
    self->ballPos[3] = target[3];
    break;
  case 1:
    npc->timer = npc->timer - 1;
    if (npc->timer == 0) {
      npc->subStep = 2;
    }
    break;
  case 2:
    target[0] = self->ballPos[0];
    target[1] = self->ballPos[1];
    target[2] = self->ballPos[2];
    target[3] = self->ballPos[3];
    turn = ActorTurnToward(atan2f(target[2] - npc->base.base.pos[2], target[0] - npc->base.base.pos[0]),
                           1.0f, 0.13962634f, self);
    if (turn * turn < 0.01f) {
      ActorPlayMotion(0.2f, self, 0xb, 0, 0);
    } else {
      ActorPlayMotion(0.2f, self, 8, 0, 0);
      npc->subStep = 3;
    }
    break;
  case 3:
    entry = (const VtblEntry *)npc->base.base.base.vtable + 18;
    if (((s32 (*)(void *, float *, s32, s32, float, float))entry->fn)(
            (char *)self + entry->delta, self->ballPos, 0, 0, 0.5f, 12.0f) != 0) {
      npc->subStep = 4;
    }
    break;
  case 4:
    npc->timer = 2;
    npc->subStep = 5;
    break;
  case 5:
    ActorPlayMotion(0.2f, self, 0xd, 0, 0);
    timer = npc->timer;
    npc->subStep = 6;
    npc->timer = timer - 1;
    /* fall through */
  case 6:
    if (npc->base.base.motionEnded != 0) {
      if (npc->timer > 0) {
        ActorPlayMotion(0.0f, self, 0, 0, 1);
        npc->subStep = 5;
      } else {
        ActorPlayMotion(0.2f, self, 0, 0, 0);
        npc->subStep = 7;
      }
    }
    break;
  case 7:
    target[0] = npc->returnPoint[0];
    target[1] = npc->returnPoint[1];
    target[2] = npc->returnPoint[2];
    target[3] = npc->returnPoint[3];
    turn = ActorTurnToward(atan2f(target[2] - npc->base.base.pos[2], target[0] - npc->base.base.pos[0]),
                           1.0f, 0.13962634f, self);
    if (turn * turn < 0.01f) {
      ActorPlayMotion(0.2f, self, 0xb, 0, 0);
    } else {
      ActorPlayMotion(0.2f, self, 8, 0, 0);
      npc->subStep = 8;
    }
    break;
  case 8:
    entry = (const VtblEntry *)npc->base.base.base.vtable + 18;
    if (((s32 (*)(void *, float *, s32, s32, float, float))entry->fn)(
            (char *)self + entry->delta, npc->returnPoint, 0, 0, 0.5f, 0.0f) != 0) {
      npc->subStep = 9;
    }
    break;
  case 9:
    turn = ActorTurnToward(npc->homeHeading, 1.0f, 0.13962634f, self);
    if (turn * turn < 0.01f) {
      npc->timer = 30;
      npc->subStep = 10;
      ActorPlayMotion(0.2f, self, 0, 0, 0);
    } else {
      ActorPlayMotion(0.2f, self, 8, 0, 0);
    }
    break;
  case 10:
    if (npc->timer == 0) {
      ActorNpcShowHeadEffect(npc, 0x29, 0, 1);
      ((ActorNpcViewCone *)npc->viewCone)->visible = 1;
      npc->aiState = 0;
      npc->subStep = 0;
    } else {
      npc->timer = npc->timer - 1;
      ActorNpcSetEffectAlpha((float)(npc->timer % 2), npc, 0x29);
    }
    break;
  }
}
