// bdc 0x089923a0 UiUnlockCodeGetDigitCount
#include "bdc.h"

/* Returns the number of digits of the code being entered: 10 in Bakugan Dimensions mode (`+0x108`
   != 0), else 8 — matching the `DWSpecialUnlock` texts "Enter your exclusive 8-digit unlock code"
   / "Enter up to 8 of your unique 10-digit codes". */

s32 UiUnlockCodeGetDigitCount(UiUnlockCode *self)

{
  s32 digits;
  
  digits = 10;
  if (self->dimensionsMode == 0) {
    digits = 8;
  }
  return digits;
}

