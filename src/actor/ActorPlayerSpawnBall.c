// bdc 0x088e1968 ActorPlayerSpawnBall
#include "bdc.h"

/* Creates the ball the player throws (`ball`, `ActorBallCreate``(0.1, kind, 3, 0)`) on first
   use; when `pos` is given, copies the vec4 into the ball's `pos` and sets its heading to `pos[3]`
   (`ActorBallSetHeading`); always remembers `kind` in `ballKind`. Used by `ActorPlayerStateThrow`. */

void ActorPlayerSpawnBall(ActorPlayer *self, s32 kind, const float *pos)
{
  GfxModel *ball;

  if (self->ball == NULL) {
    self->ball = ActorBallCreate(0.1f, kind, 3, NULL);
  }
  if (pos != NULL) {
    ball = (GfxModel *)self->ball;
    ball->pos[0] = pos[0];
    ball->pos[1] = pos[1];
    ball->pos[2] = pos[2];
    ball->pos[3] = pos[3];
    ActorBallSetHeading(pos[3], (GfxModel *)self->ball);
  }
  self->ballKind = kind;
}
