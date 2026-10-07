// bdc 0x0892f440 UiBakuganSelectFlashDone
#include "bdc.h"

/* Advances both flash channels (`UiFlashStep`) and returns 1 once channel 0 has completed (0 while it is still running); the channel 1 result is ignored. */

int UiBakuganSelectFlashDone(void)
{
  UiFlashStep(1);
  if (UiFlashStep(0) == 0) {
    return 0;
  }
  return 1;
}
