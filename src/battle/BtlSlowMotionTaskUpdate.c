// bdc 0x088493c8 BtlSlowMotionTaskUpdate
#include "bdc.h"

/* Update of the slow-motion task (id 0x14b): on its first frame (`started` clear) sets the battle
   main task's white-flash target alpha `flashTarget` (`+0x4e0`) to 0.85, sets the global motion
   time scale to 0.55 (`GfxSetMotionTimeScale`) and marks the task started. */

void BtlSlowMotionTaskUpdate(BtlSlowMotionTask *task)
{
  BtlMain *battle;

  if (task->started == 0) {
    battle = (BtlMain *)BtlGetCameraTask();
    battle->flashTarget = 0.850000024f; /* 0x3f59999a */
    GfxSetMotionTimeScale(0.550000012f); /* 0x3f0ccccd */
    task->started = 1;
  }
}
