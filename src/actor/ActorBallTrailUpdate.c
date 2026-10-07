// bdc 0x088b7c9c ActorBallTrailUpdate
#include "bdc.h"

/* Per-frame update of the ball trail (`ActorBallTrail`), run by `ActorPlayerUpdate`: with a ball
   attached (`ball`) adds a segment from the ball's position to `lastPos`
   (`ActorBallTrailAddSegment`) and copies the ball position into `lastPos`; then walks the 192 sprites: while the trail is enabled
   (`enable`) a sprite whose `scaleX` is still above 0 gets alpha 0.1 and shrinks by 1, otherwise
   its alpha drops by 0.01; alpha is then clamped to [0, 0.15]. Without a ball nothing happens. */

void ActorBallTrailUpdate(ActorBallTrail *trail)
{
  ActorBallTrail *self = trail;
  float *pos;
  GfxSprite *spr;
  float alpha;
  s32 i;

  if (self->ball == NULL) {
    return;
  }
  ActorBallTrailAddSegment(self, self->ball->base.pos, self->lastPos);
  pos = self->ball->base.pos;
  self->lastPos[0] = pos[0];
  self->lastPos[1] = pos[1];
  self->lastPos[2] = pos[2];
  self->lastPos[3] = pos[3];
  for (i = 0; i < 192; i++) {
    spr = (GfxSprite *)self->sprites[i];
    if (spr == NULL) {
      continue;
    }
    if (self->enable != 0 && !(spr->scaleX <= 0.0f)) {
      spr->alpha = 0.1f;
      spr->scaleX = spr->scaleX - 1.0f;
    } else {
      spr->alpha = spr->alpha - 0.01f;
    }
    alpha = spr->alpha;
    if (alpha < 0.0f) {
      alpha = 0.0f;
    } else if (!(alpha <= 0.15f)) {
      alpha = 0.15f;
    }
    spr->alpha = alpha;
  }
}
