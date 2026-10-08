// bdc 0x089dd470 GmoDlWriteResetState
#include "bdc.h"

/* For models with flag 2 (`flags28`), appends the GE state-reset sequence. When every state group
   in `0x0ff0ffff` is set in `stateMask`, it reserves 2 words and writes `BASE` + `CALL` to the static
   list `g_gmoResetStateDl`; otherwise it reserves 31 words and copies the list's 31 commands, zeroing
   (NOP) those whose group is inactive. Command bit `i` of `0x4643fbff` marks a command that starts
   a new group (taking the next `stateMask` group bit, bits 0-15 then 20-27); the others share the
   previous group's keep/zero choice. `cur` is advanced before the capacity check: on overflow
   nothing is written. */

void GmoDlWriteResetState(GmoDlContext *self, GmoModel *model)
{
    const u32 *src;
    u32 *dl;
    u32 *end;
    u32 mask;
    u32 addr;
    u32 starts;
    s32 groups;
    u32 keep;
    int i;

    if ((model->flags28 & 2) == 0) {
        return;
    }
    dl = self->cur;
    mask = self->stateMask;
    src = g_gmoResetStateDl;
    end = self->end;
    if ((~mask & 0x0ff0ffff) == 0) {
        self->cur = dl + 2;
        if (end < dl + 2) {
            return;
        }
        addr = PspAddr(src); /* GE address of the list */
        *dl++ = (addr >> 24) << 16 | 0x10000000;
        *dl++ = (addr & 0xffffff) | 0x0a000000;
        return;
    }
    self->cur = dl + 31;
    if (end < dl + 31) {
        return;
    }
    groups = (s32)(((s32)mask >> 4 & 0xffff0000) | (mask & 0xffff));
    keep = 0;
    starts = 0x4643fbff;
    for (i = 0; i < 31; i++) {
        if (starts & 1) {
            keep = (groups & 1) ? 0xffffffff : 0;
            groups >>= 1;
        }
        *dl++ = src[i] & keep;
        starts >>= 1;
    }
}
