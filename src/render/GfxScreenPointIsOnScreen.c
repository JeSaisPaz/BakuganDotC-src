// bdc 0x089beec0 GfxScreenPointIsOnScreen
#include "bdc.h"

/* Returns 1 when the projected point lies strictly inside the 480x272 screen (`0 < x < 480`, `0 < y
   < 272`), else 0. */

int GfxScreenPointIsOnScreen(float *pt)
{
  if (!(pt[0] <= 0.0f) && pt[0] < 480.0f && !(pt[1] <= 0.0f) && pt[1] < 272.0f) {
    return 1;
  }
  return 0;
}
