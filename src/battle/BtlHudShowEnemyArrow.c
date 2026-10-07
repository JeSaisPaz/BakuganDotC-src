// bdc 0x088307b0 BtlHudShowEnemyArrow
#include "bdc.h"

/* Shows or hides off-screen enemy arrow `dir` (0..3) of the battle HUD (`BtlHudUpdate`); its
   sprites are `sprites[15 + 3*dir .. 17 + 3*dir]`, its palette blender `arrowBlend[dir]`. While the
   HUD is visible (`BtlHudIsVisible`) the three sprites first lose their visible flag (bit 0).
   `mode` 0 clears `arrowShown[dir]` and `arrowWarn[dir]` (only when the arrow was shown). Any other
   `mode` sets `arrowShown[dir]`; 1 also sets `arrowWarn[dir]` and selects palette row 0, 2 clears
   `arrowWarn[dir]` and selects row 1, other values keep `arrowWarn` and use row 0; then, only while
   the HUD is not visible, the first sprite gets its visible flag set, and the row is applied with
   `GfxPaletteBlendSetRow`. */

void BtlHudShowEnemyArrow(BtlHud *self, s32 dir, s32 mode)
{
    s32 row = 0;
    int i;

    if (BtlHudIsVisible()) {
        for (i = 0; i < 3; i++) {
            GfxSprite *sprite = self->sprites[15 + dir * 3 + i];

            sprite->flags &= ~1u;
        }
    }
    if (mode == 0) {
        if (self->arrowShown[dir] != 0) {
            self->arrowShown[dir] = 0;
            self->arrowWarn[dir] = 0;
        }
        return;
    }
    if (mode == 1) {
        self->arrowWarn[dir] = 1;
        row = 0;
    } else if (mode == 2) {
        row = 1;
        self->arrowWarn[dir] = 0;
    }
    if (self->arrowShown[dir] == 0) {
        self->arrowShown[dir] = 1;
    }
    for (i = 0; i < 1; i++) {
        if (!BtlHudIsVisible()) {
            GfxSprite *sprite = self->sprites[15 + dir * 3 + i];

            sprite->flags |= 1;
        }
    }
    GfxPaletteBlendSetRow(self->arrowBlend[dir], row);
}
