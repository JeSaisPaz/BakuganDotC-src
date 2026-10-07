// bdc 0x08854f38 ActorCrystalUvScrollStep
#include "bdc.h"

/* Advances a `{pos, speed}` scroller: `pos += speed`, wrapped into [0, 1). */
void ActorCrystalUvScrollStep(float *scroll)
{
  scroll[0] = scroll[0] + scroll[1];
  if (scroll[0] < 0.0f) {
    scroll[0] = scroll[0] + 1.0f;
  }
  if (!(scroll[0] < 1.0f)) {
    scroll[0] = scroll[0] - 1.0f;
  }
}
