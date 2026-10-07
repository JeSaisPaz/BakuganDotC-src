// bdc 0x0893a728 UiUnlockResultPiecesDone
#include "bdc.h"

/* Advances the animations started by `UiUnlockResultShowPieces` on
   `UiUnlockResult`: for reward kind `+0x5ee` 2 pops the `+0x637` digit sprites
   from 8 on, for kind 5 sprites 0x0f–0x11 (`UiUnlockResultPopSprite`); returns true when any
   reports finished (always true for other kinds). */

bool UiUnlockResultPiecesDone(UiUnlockResult *self, u8 closing)
{
    u8 done = 0;
    s32 i;
    s32 sum;

    if (self->rewardKind == 2) {
        for (i = 0; i < (s32)self->digitCount; i++) {
            done += UiUnlockResultPopSprite(self, closing, (u8)(i + 8));
        }
    } else if (self->rewardKind == 5) {
        sum = UiUnlockResultPopSprite(self, closing, 0xf);
        sum += UiUnlockResultPopSprite(self, closing, 0x10);
        sum += UiUnlockResultPopSprite(self, closing, 0x11);
        if (sum > 0) {
            done = 1;
        }
    } else {
        done = 1;
    }
    return done != 0;
}
