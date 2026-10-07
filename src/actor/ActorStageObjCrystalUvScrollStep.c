// bdc 0x088b3f9c ActorStageObjCrystalUvScrollStep
#include "bdc.h"

/* Advances a UV scroll pair `{offset, speed}`: `offset += speed`, wrapped into [0, 1). Crystal copy
   used by `ActorStageObjCrystalStepUvScroll`. */
void ActorStageObjCrystalUvScrollStep(float *scroll)
{
  scroll[0] = scroll[0] + scroll[1];
  if (scroll[0] < 0.0f) {
    scroll[0] = scroll[0] + 1.0f;
  }
  if (!(scroll[0] < 1.0f)) {
    scroll[0] = scroll[0] - 1.0f;
  }
}
