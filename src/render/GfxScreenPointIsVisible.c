// bdc 0x089bef28 GfxScreenPointIsVisible
#include "bdc.h"

/* Returns 1 when a projected point is horizontally within `margin` pixels of the screen (`-margin <
   x < 480 + margin`) and in front of the camera (`z < -0.4`), else 0. Used by the distance-fade
   code (`ActorUpdateDistanceFade`, `ActorStageObjUpdateFade`). */

int GfxScreenPointIsVisible(float margin, float *pt)
{
  if (!(pt[0] <= -margin) && pt[0] < margin + 480.0f && pt[2] < -0.4f) {
    return 1;
  }
  return 0;
}
