// bdc 0x0882d0fc BtlHudUpdateHpBar
#include "bdc.h"

/* Sizes the HP bar frame of the battle HUD (`BtlHudUpdate`) to the Bakugan's maximum HP
   (`BtlCombatGetMaxHp`, converted to float as an unsigned value): the bar end is
   `(int)(((maxHp / levelHp[4] - 0.5) * 0.7 + 0.3) * 176 + 24)` with `levelHp[4]` from the unit's
   stat table; HUD sprite 0x6a (the frame) gets width `end - 24` at its own height
   (`GfxSpriteGetHeight`, `GfxSpriteSetSize`) and its end cap, sprite 0x6b, moves to
   `frame x + end - 24`. Does nothing without a unit. */

void BtlHudUpdateHpBar(BtlHud *self, void *unit)
{
    BtlBakugan *bakugan = unit;
    GfxSprite *frame;
    float maxHp;
    float end;

    if (bakugan == NULL) {
        return;
    }
    maxHp = (float)(u32)BtlCombatGetMaxHp(&bakugan->combat);
    frame = self->sprites[0x6a];
    end = (float)(int)(((maxHp / bakugan->combat.stats->levelHp[4] - 0.5f) * 0.699999988f +
                        0.300000012f) * 176.0f + 24.0f);
    GfxSpriteSetSize(frame, end - 24.0f, GfxSpriteGetHeight(frame));
    self->sprites[0x6b]->posX = frame->posX + end - 24.0f;
}
