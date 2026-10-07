// bdc 0x0893a188 UiUnlockResultRewardPopDone
#include "bdc.h"

/* Advances the reward animation started by `UiUnlockResultStartRewardPop` on
   `UiUnlockResult`: `UiUnlockResultPopSprite` on sprite 0x0c for 2D rewards,
   `UiUnlockResultUpdateModelFade` for reward kind `+0x5ee` 5/6. Returns true when finished. */

bool UiUnlockResultRewardPopDone(UiUnlockResult *self, u8 closing)

{
  bool done;
  
  done = false;
  if ((self->rewardKind != '\x05') && (done = false, self->rewardKind != '\x06')) {
    done = true;
  }
  if (!done) {
    done = UiUnlockResultUpdateModelFade(self,closing);
    return done;
  }
  done = UiUnlockResultPopSprite(self,closing,'\f');
  return done;
}

