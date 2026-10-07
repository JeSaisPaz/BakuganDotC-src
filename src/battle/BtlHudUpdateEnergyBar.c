// bdc 0x0882d200 BtlHudUpdateEnergyBar
#include "bdc.h"

/* Updates the energy gauge of the battle HUD (`BtlHudUpdate`) for `unit` (nothing when NULL): sets
   the frame sprite `sprites[0x68]` to 52 px wide (its own height), places the end cap
   `sprites[0x69]` at the frame's X + 72 − 20, and crops the fill sprite `sprites[0xc]` to
   `energy × 0.001 × 77` px (`BtlHudSetSpriteRect`, full sprite height); a fill below 1 px is
   raised to 1 unless the energy (`BtlCombatGetEnergy`) is at or below 0. */

void BtlHudUpdateEnergyBar(BtlHud *self, BtlBakugan *unit)
{
    GfxSprite *sprite;
    float energy;
    float fill;
    float barLen;

    if (unit == NULL) {
        return;
    }
    energy = BtlCombatGetEnergy(&unit->combat);
    barLen = (float)(s32)72.0f;
    sprite = self->sprites[0x68];
    GfxSpriteSetSize(sprite, barLen - 20.0f, GfxSpriteGetHeight(sprite));
    self->sprites[0x69]->posX = (sprite->posX + barLen) - 20.0f;
    fill = energy * 0.00100000005f * 77.0f;
    if (fill < 1.0f && !(energy <= 0.0f)) {
        fill = 1.0f;
    }
    sprite = self->sprites[0xc];
    BtlHudSetSpriteRect(0.0f, 0.0f, fill, GfxSpriteGetHeight(sprite), self, sprite);
}
