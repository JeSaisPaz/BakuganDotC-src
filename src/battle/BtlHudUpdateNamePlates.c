// bdc 0x088333c0 BtlHudUpdateNamePlates
#include "bdc.h"

/* HUD widget updater (`BtlHudPhaseMain`) for the 3 Bakugan name plates (layout sprites
   0xbc..0xbe, texture "game_baku_name", 128x16 cells): gets unit `i`
   (`BtlHudGetNthOtherBakugan` in battle rule mode 2, else `BtlHudGetEnemyUnit`); when there
   is none or `BtlHudIsNamePlateHidden` says so, hides the plate (clears flags bit 0), otherwise
   selects row `(kind - 1) % 32` of the name sheet (kind = the unit object's `unk08`,
   `GfxSpriteSetCell`) and copies the unit's ambient alpha to the plate alpha. The binary also
   stores `kind - 1` (or -1) into a 3-word stack array that is never read; it is omitted. */
void BtlHudUpdateNamePlates(BtlHud *self)
{
    BtlBakugan *unit;
    GfxSprite *plate;
    s32 i;

    for (i = 0; i < 3; i++) {
        if (g_scriptGlobalVars[8] == 2) {
            unit = (BtlBakugan *)BtlHudGetNthOtherBakugan(self, i);
        } else {
            unit = (BtlBakugan *)BtlHudGetEnemyUnit(self, i);
        }
        if (unit == NULL || BtlHudIsNamePlateHidden(self, unit) != 0) {
            plate = self->sprites[0xbc + i];
            plate->flags &= ~1u;
        } else {
            GfxSpriteSetCell(self->sprites[0xbc + i], 0.0f,
                             (float)((s32)(unit->base.base.unk08 - 1) % 32));
            self->sprites[0xbc + i]->alpha = unit->base.ambient[3];
        }
    }
}
