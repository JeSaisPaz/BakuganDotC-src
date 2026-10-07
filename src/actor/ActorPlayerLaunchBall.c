// bdc 0x088e3af4 ActorPlayerLaunchBall
#include "bdc.h"

/* Releases the ball `ball` (`ActorBallCreate`) from the hand (`ActorPlayerGetThrowHandPos`,
   `rightHand` forced on) toward the aim point `aimPoint`: flight time 18 frames (9 when
   `aimTargetKind == 2`), horizontal velocity = distance * (1 / frames) (VFPU `vrcp.s`), vertical
   velocity solving the arc under gravity 0.04 per frame, or with the summed 0.08 steps of
   `ActorSumArithmeticSeries` when a lock target `lockTarget` is set. Remembers the start point in
   `ballStart`, sets the ball's `owner`, starts the throw (`ActorBallBeginThrow`) and, when there is
   a trail, sets `throwing` and restarts the trail on the ball (`ActorBallTrailReset`). Does nothing
   without a ball. */

void ActorPlayerLaunchBall(ActorPlayer *self)
{
  float hand[4];
  float vel[4];
  float aim[4];
  ActorBall *ball;
  ActorBallTrail *trail;
  s32 frames;
  float fframes;
  float k;
  float vy;

  if (self->ball == NULL) {
    return;
  }
  frames = 18;
  aim[0] = self->base.aimPoint[0];
  aim[1] = self->base.aimPoint[1];
  aim[2] = self->base.aimPoint[2];
  aim[3] = self->base.aimPoint[3];
  if (self->base.aimTargetKind == 2) {
    frames = 9;
  }
  self->rightHand = 1;
  ball = (ActorBall *)self->ball;
  ActorPlayerGetThrowHandPos(self, hand);

  ball->base.pos[0] = hand[0];
  ball->base.pos[1] = hand[1];
  ball->base.pos[2] = hand[2];
  ball->base.pos[3] = hand[3];
  /* vsub.t keeps lane 3 of the aim vector. */
  vel[0] = aim[0] - ball->base.pos[0];
  vel[1] = aim[1] - ball->base.pos[1];
  vel[2] = aim[2] - ball->base.pos[2];
  vel[3] = aim[3];
  fframes = (float)frames;
  k = 1.0f / fframes;
  vel[0] = vel[0] * k;
  vel[1] = vel[1] * k;
  vel[2] = vel[2] * k;
  self->ballStart[0] = ball->base.pos[0];
  self->ballStart[1] = ball->base.pos[1];
  self->ballStart[2] = ball->base.pos[2];
  self->ballStart[3] = ball->base.pos[3];
  ball->owner = self;
  ball->base.velocity[0] = vel[0];
  ball->base.velocity[1] = vel[1];
  ball->base.velocity[2] = vel[2];
  ball->base.velocity[3] = vel[3];

  if (self->lockTarget != NULL) {
    float dy = aim[1] - ball->base.pos[1];
    s32 sum = ActorSumArithmeticSeries(1, 1, frames + 1);

    vy = (dy - (float)sum * -0.0799999982f) / fframes;
  } else {
    vy = ((aim[1] - ball->base.pos[1]) - fframes * -0.0399999991f * fframes) / fframes;
  }
  ball->base.velocity[1] = vy;
  ActorBallBeginThrow(&ball->base.base);

  if (self->trail != NULL) {
    trail = (ActorBallTrail *)self->trail;
    self->throwing = 1;
    trail->ball = ball;
    ActorBallTrailReset(trail, true);
  }
}
