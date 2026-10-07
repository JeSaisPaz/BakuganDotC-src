// bdc 0x0888f314 BtlAiPadSteer
#include "bdc.h"

/* Converts a relative 16-bit direction `dir` (0 = forward, +/-0x8000 = back) into stick presses
   on the virtual pad of `BtlAi`. While the side flag 0x20000 of `moveFlags` is
   set, `m = 0x800` (else 0), which narrows the forward and back windows and widens the sideways
   ones: forward (`BtlAiPadStickForward`) for `m - 0x3000 <= dir <= 0x3000 - m`, else back
   (`BtlAiPadStickBack`) for `|dir| >= 0x5000 + m`; sideways `BtlAiPadStickDir4000` for
   `0x1000 - m <= dir <= 0x7000 + m`, else `BtlAiPadStickDirC000` for `-(0x7000 + m) <= dir <=
   m - 0x1000`, either one setting the side flag, which is cleared otherwise. `hold` (only
   honoured with `allowedCmds` bit 4) also holds button bit 4 (`BtlAiPadPress04`). Does nothing
   unless `allowedCmds` bit 2 (move) or `hold`. */
void BtlAiPadSteer(BtlAi *self, s16 dir, u8 hold)
{
    s32 d = dir;
    s32 m = 0;

    if ((self->moveFlags & 0x20000) != 0) {
        m = 0x800;
    }
    if ((self->allowedCmds & 4) == 0) {
        hold = 0;
    }
    if ((self->allowedCmds & 2) == 0 && hold == 0) {
        return;
    }
    if (d >= m - 0x3000 && d <= 0x3000 - m) {
        BtlAiPadStickForward(&self->pad);
    } else if (d >= m + 0x5000 || d <= -(m + 0x5000)) {
        BtlAiPadStickBack(&self->pad);
    }
    if (d >= 0x1000 - m && d <= m + 0x7000) {
        BtlAiPadStickDir4000(&self->pad);
        self->moveFlags |= 0x20000;
    } else if (d >= -(m + 0x7000) && d <= m - 0x1000) {
        BtlAiPadStickDirC000(&self->pad);
        self->moveFlags |= 0x20000;
    } else {
        self->moveFlags &= ~0x20000u;
    }
    if (hold != 0) {
        BtlAiPadPress04(&self->pad);
    }
}
