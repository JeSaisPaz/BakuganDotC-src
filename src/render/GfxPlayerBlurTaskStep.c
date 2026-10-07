// bdc 0x0882ae44 GfxPlayerBlurTaskStep
#include "bdc.h"

/* State machine of the `GfxPlayerBlurTask` (`state`), run once per frame; returns 1 when it
   removed the task, else 0. State 0 fades `alpha` in by 0.1 per frame (at `!(alpha < 1)` clamps
   to 1 and goes to 1) and, while `frameCounter` is 1, installs `GfxCopyFrameToBuffer` as the
   display signal hook; 1 holds until `holdCounter` (post-incremented) exceeds `holdFrames`
   (forever when `holdFrames` is negative); 2 fades out by 0.05 (at `<= 0` clamps to 0 and goes
   to 3); 3 goes to 4; 4 and 5 remove the task (`CoreTaskRemove`, destroy) and return 1 without
   counting the frame. Other states only count. Every 0 return increments `frameCounter`. */

s32 GfxPlayerBlurTaskStep(GfxPlayerBlurTask *task)
{
  s32 held;
  float alpha;

  switch (task->state) {
  case 0:
    alpha = task->alpha + 0.1f;
    task->alpha = alpha;
    if (!(alpha < 1.0f)) {
      task->alpha = 1.0f;
      task->state = task->state + 1;
    }
    if (task->frameCounter == 1) {
      g_gfxDisplay->signalHook = GfxCopyFrameToBuffer;
    }
    break;
  case 1:
    if (task->holdFrames >= 0) {
      held = task->holdCounter;
      task->holdCounter = held + 1;
      if (task->holdFrames < held) {
        task->state = task->state + 1;
      }
    }
    break;
  case 2:
    alpha = task->alpha - 0.05f;
    task->alpha = alpha;
    if (alpha <= 0.0f) {
      task->alpha = 0.0f;
      task->state = task->state + 1;
    }
    break;
  case 3:
    task->state = task->state + 1;
    break;
  case 4:
  case 5:
    CoreTaskRemove(&task->base, true);
    return 1;
  default:
    break;
  }
  task->frameCounter = task->frameCounter + 1;
  return 0;
}
