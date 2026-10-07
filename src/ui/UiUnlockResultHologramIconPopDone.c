// bdc 0x0893a2bc UiUnlockResultHologramIconPopDone
#include "bdc.h"

/* Advances the pop of sprite 6 on `UiUnlockResult` for reward kind `+0x5ee` 2
   (`UiUnlockResultPopSprite`); returns true at once for other kinds. */

bool UiUnlockResultHologramIconPopDone(UiUnlockResult *self, u8 closing)

{
  if (self->rewardKind == '\x02') {
    return UiUnlockResultPopSprite(self,closing,'\x06');
  }
  return true;
}

