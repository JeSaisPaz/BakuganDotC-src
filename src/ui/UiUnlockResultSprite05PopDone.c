// bdc 0x0893a8a0 UiUnlockResultSprite05PopDone
#include "bdc.h"

/* Advances the pop of sprite 5 on `UiUnlockResult` for reward kind `+0x5ee` 1,
   2, 3, 7; true at once otherwise. */

bool UiUnlockResultSprite05PopDone(UiUnlockResult *self, u8 closing)

{
  bool done;
  
  switch(self->rewardKind) {
  case '\0':
  case '\x04':
  case '\b':
  case '\t':
    return true;
  case '\x01':
  case '\x02':
  case '\x03':
  case '\a':
    done = UiUnlockResultPopSprite(self,closing,'\x05');
    return done;
  case '\x05':
  case '\x06':
    return true;
  default:
    return true;
  }
}

