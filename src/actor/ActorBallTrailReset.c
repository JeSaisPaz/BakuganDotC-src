// bdc 0x088b77c8 ActorBallTrailReset
#include "bdc.h"

/* Resets the ball trail (0x840 bytes, `ActorBallTrailCtor`): no segment yet (`segIndex = -1`);
   for each non-NULL sprite of the 192-entry table `sprites` it sets `scaleX = 30`, sizes it to
   `10 * ball scale` (width/height/depth, `maybe_sizeW = 0`) when a ball is attached, and when
   `hide` is set also clears its alpha and parks it at (0, -1000, 0, 0). With a ball, the last
   position `lastPos` is reset to (0, -1000, 0, 0). */

void ActorBallTrailReset(ActorBallTrail *trail, bool hide)
{
  GfxSprite *sprite;
  float *pos;
  float size;
  int i;

  trail->segIndex = -1;
  for (i = 0; i < 0xc0; i++) {
    sprite = (GfxSprite *)trail->sprites[i];
    if (sprite != NULL) {
      if (hide) {
        sprite->alpha = 0.0f;
        pos = &sprite->posX;
        pos[0] = 0.0f;
        pos[1] = -1000.0f;
        pos[2] = 0.0f;
        pos[3] = 0.0f;
      }
      if (trail->ball != NULL) {
        size = trail->ball->base.scale[0] * 10.0f;
        sprite->maybe_sizeW = 0.0f;
        sprite->width = size;
        sprite->height = size;
        sprite->depth = size;
      }
      sprite->scaleX = 30.0f;
    }
  }
  if (trail->ball != NULL) {
    trail->lastPos[0] = 0.0f;
    trail->lastPos[1] = -1000.0f;
    trail->lastPos[2] = 0.0f;
    trail->lastPos[3] = 0.0f;
  }
}
