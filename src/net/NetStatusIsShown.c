// bdc 0x08943aac NetStatusIsShown
#include "bdc.h"

/* Returns 1 when the netplay status overlay `NetStatusTask` (task 0x7d2) exists
   and its dimming alpha (`alpha`) is not <= 0 (NaN counts as shown), else 0. */

s32 NetStatusIsShown(void)

{
  NetStatusTask *task;
  s32 shown;

  shown = 0;
  task = (NetStatusTask *)CoreTaskFind(0x7d2);
  if ((task != NULL) && !(task->alpha <= 0.0f)) {
    shown = 1;
  }
  return shown;
}
