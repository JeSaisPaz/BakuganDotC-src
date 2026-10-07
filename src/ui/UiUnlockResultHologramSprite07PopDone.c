// bdc 0x0893a374 UiUnlockResultHologramSprite07PopDone
#include "bdc.h"

/* Advances the pop of sprite 7 on `UiUnlockResult` for reward kind `+0x5ee` 2;
   returns true at once for other kinds. */

bool UiUnlockResultHologramSprite07PopDone(UiUnlockResult *self, u8 closing)

{
  if (self->rewardKind == '\x02') {
    return UiUnlockResultPopSprite(self,closing,'\a');
  }
  return true;
}

