// bdc 0x089394e4 UiUnlockResultInitDigits
#include "bdc.h"

/* Prepares the number display of `UiUnlockResult` for reward kind `+0x5ee` 2
   (hologram): sets the value `+0x634` to 500 and the digit cell `+0x636` to 4, converts it to
   digits in `+0x630` (`UiNumberToDigits`, 4 digits, 0xff-terminated) and stores the digit count
   in `+0x637`; for other kinds both words are zeroed. */

void UiUnlockResultInitDigits(UiUnlockResult *self)
{
  u32 i;

  memset(self->digits, 0, 4);
  memset(&self->digitValue, 0, 4);
  if (self->rewardKind == 2) {
    self->digitValue = 500;
    self->holoIconCell = 4;
    UiNumberToDigits(self->digits, self->digitValue, 4, 0xff);
    self->digitCount = 0;
    i = self->digitCount;
    if (self->digits[i] != 0xff) {
      do {
        i = (i + 1) & 0xff;
      } while (self->digits[i] != 0xff);
      self->digitCount = (u8)i;
    }
  }
}
