// bdc 0x0893a9c4 UiUnlockResultNamePlatePopDone
#include "bdc.h"

/* Advances the pop of name plate sprite 1 on `UiUnlockResult` for reward kind
   `+0x5ee` 1, 2, 6, 8, 9; true at once otherwise. */

bool UiUnlockResultNamePlatePopDone(UiUnlockResult *self, u8 closing)

{
  bool done;
  
  switch(self->rewardKind) {
  default:
    return true;
  case '\x01':
  case '\x02':
  case '\b':
  case '\t':
    break;
  case '\x05':
    return true;
  case '\x06':
    break;
  }
  done = UiUnlockResultPopSprite(self,closing,'\x01');
  return done;
}

