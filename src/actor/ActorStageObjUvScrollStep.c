// bdc 0x088a9134 ActorStageObjUvScrollStep
#include "bdc.h"

/* Advances a UV scroll pair `{offset, speed}` for `ActorStageObjStepUvScrolls`: adds
   `scroll[1]` to `scroll[0]`, then wraps it toward [0, 1): +1 when below 0, then -1 unless below
   1 (a NaN offset also gets -1, as in the compiled `c.lt.s`/`bc1t`). */

void ActorStageObjUvScrollStep(float *scroll)
{
  scroll[0] = scroll[0] + scroll[1];
  if (scroll[0] < 0.0f) {
    scroll[0] = scroll[0] + 1.0f;
  }
  if (!(scroll[0] < 1.0f)) {
    scroll[0] = scroll[0] - 1.0f;
  }
}
