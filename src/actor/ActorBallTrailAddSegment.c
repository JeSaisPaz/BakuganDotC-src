// bdc 0x088b7adc ActorBallTrailAddSegment
#include "bdc.h"

/* Adds a segment to the ball trail: the first call (`segIndex == -1`) starts at slot 0; later
   calls advance the 64-entry ring (`segIndex = (segIndex + 1) & 63`) only while the trail is
   enabled (`enable`), otherwise they return without touching anything. Stores `pos` in
   `segPos[i]` and `prevPos` in `segPrevPos[i]`, then places the slot's three sprites
   (`sprites[i*3..i*3+2]`) at `pos` and at 0.33333 and 0.66666 of the way to `prevPos`, each with
   `scaleX = 30`. */

/* point = from + (to - from) * factor, written to `dst` (per component, as vsub/vscl/vadd.q). */
static inline void ActorBallTrailLerpToSprite(float *dst, const float *from, const float *to,
                                              float factor)
{
  float mid[4];
  int k;

  for (k = 0; k < 4; k++) {
    mid[k] = from[k] + (to[k] - from[k]) * factor;
  }
  for (k = 0; k < 4; k++) {
    dst[k] = mid[k];
  }
}

void ActorBallTrailAddSegment(ActorBallTrail *trail, float *pos, float *prevPos)
{
  GfxSprite *sprite;
  int idx;

  if (trail->segIndex == -1) {
    trail->segIndex = 0;
    trail->segPos[0][0] = pos[0];
    trail->segPos[0][1] = pos[1];
    trail->segPos[0][2] = pos[2];
    trail->segPos[0][3] = pos[3];
  } else {
    if (trail->enable == 0) {
      return;
    }
    trail->segIndex = trail->segIndex + 1;
    idx = trail->segIndex & 0x3f;
    trail->segIndex = idx;
    trail->segPos[idx][0] = pos[0];
    trail->segPos[idx][1] = pos[1];
    trail->segPos[idx][2] = pos[2];
    trail->segPos[idx][3] = pos[3];
  }
  idx = trail->segIndex;
  trail->segPrevPos[idx][0] = prevPos[0];
  trail->segPrevPos[idx][1] = prevPos[1];
  trail->segPrevPos[idx][2] = prevPos[2];
  trail->segPrevPos[idx][3] = prevPos[3];

  idx = trail->segIndex;
  sprite = (GfxSprite *)trail->sprites[idx * 3];
  sprite->posX = trail->segPos[idx][0];
  sprite->posY = trail->segPos[idx][1];
  sprite->posZ = trail->segPos[idx][2];
  sprite->posW = trail->segPos[idx][3];

  sprite = (GfxSprite *)trail->sprites[idx * 3 + 1];
  ActorBallTrailLerpToSprite(&sprite->posX, trail->segPos[idx], trail->segPrevPos[idx],
                             0.33333f);
  sprite = (GfxSprite *)trail->sprites[idx * 3 + 2];
  ActorBallTrailLerpToSprite(&sprite->posX, trail->segPos[idx], trail->segPrevPos[idx],
                             0.66666f);

  ((GfxSprite *)trail->sprites[idx * 3])->scaleX = 30.0f;
  ((GfxSprite *)trail->sprites[idx * 3 + 1])->scaleX = 30.0f;
  ((GfxSprite *)trail->sprites[idx * 3 + 2])->scaleX = 30.0f;
}
