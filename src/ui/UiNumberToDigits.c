// bdc 0x089a4b1c UiNumberToDigits
#include "bdc.h"

/* Splits `value` into `digits` decimal digits and writes them to `out` without leading zeros (the
   last digit is always written), then appends `terminator`. Used to fill digit-sprite counters
   (e.g. `UiUpgradeSetNumber`, `UiHologramGallerySetNumber`). */

void UiNumberToDigits(u8 *out, u32 value, u8 digits, u8 terminator)
{
  u8 buf[16];
  u32 pow;
  u32 i;
  u32 k;
  u8 n;
  s32 started;
  u8 d;

  pow = 1;
  for (i = 1; i < digits; i++) {
    pow = pow * 10;
  }

  k = 0;
  while (pow != 1) {
    buf[k] = (u8)(value / pow);
    value -= buf[k] * pow;
    k++;
    pow /= 10;
  }
  buf[k] = (u8)value;

  n = 0;
  started = 0;
  for (i = 0; i < digits; i++) {
    d = buf[i];
    if (i < (u32)(digits - 1)) {
      if (d != 0) {
        out[n++] = d;
        started = 1;
      } else if (started) {
        out[n++] = d;
      }
    } else {
      out[n++] = d;
    }
  }
  out[n] = terminator;
}
