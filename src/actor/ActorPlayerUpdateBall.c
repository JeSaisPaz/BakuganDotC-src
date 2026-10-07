// bdc 0x088e3f44 ActorPlayerUpdateBall
#include "bdc.h"

/* Per-frame flight of the thrown ball: steering (`ActorPlayerSteerBall`), gravity 0.08 on the
   ball's vertical velocity, position += velocity (xyz). Returns 1 when the flight is over (no ball,
   new y below -150 or speed below 0.1), otherwise the ball's hit flag `flag1d2`. */

u8 ActorPlayerUpdateBall(ActorPlayer *self)
{
  float pos[4];
  ActorBall *ball;
  float speed;
  u8 done;

  if (self->ball == NULL) {
    return 1;
  }
  ball = (ActorBall *)self->ball;
  pos[0] = ball->base.pos[0];
  pos[1] = ball->base.pos[1];
  pos[2] = ball->base.pos[2];
  pos[3] = ball->base.pos[3];
  ActorPlayerSteerBall(self);
  ball = (ActorBall *)self->ball;
  ball->base.velocity[1] = ball->base.velocity[1] + -0.08f;
  ball = (ActorBall *)self->ball;
  /* pos.xyz += velocity.xyz */
  pos[0] = pos[0] + ball->base.velocity[0];
  pos[1] = pos[1] + ball->base.velocity[1];
  pos[2] = pos[2] + ball->base.velocity[2];
  done = ((ActorBall *)self->ball)->flag1d2;
  if (pos[1] < -150.0f) {
    done = 1;
  }
  ball = (ActorBall *)self->ball;
  speed = __builtin_sqrtf(ball->base.velocity[0] * ball->base.velocity[0] +
                          ball->base.velocity[1] * ball->base.velocity[1] +
                          ball->base.velocity[2] * ball->base.velocity[2]);
  if (speed < 0.1f) {
    done = 1;
  }
  ball = (ActorBall *)self->ball;
  ball->base.pos[0] = pos[0];
  ball->base.pos[1] = pos[1];
  ball->base.pos[2] = pos[2];
  ball->base.pos[3] = pos[3];
  return done;
}
