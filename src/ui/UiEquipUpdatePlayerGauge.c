// bdc 0x08959df8 UiEquipUpdatePlayerGauge
#include "bdc.h"

/* Scrolls the texture of player `player`'s 7-part gauge on `UiEquip` (the
   Bakugan/gear loadout screen before a battle; parts from `UiEquipGetPlayerSpriteBase`): sets
   each part's UV rectangle (`GfxSpriteSetUvRectXYWH`, {x, y, w, h}, height 272): part 0
   {0, 0, 240}, parts 1-4 {0/32/0/64, scrollFast, 32/32/64/64}, parts 5-6 {0, scrollSlow, 240};
   then advances `gaugeScroll[player]` (fast +8, slow +4), each wrapping to 0 once not below
   512. It does not test the hidden value -1 itself. */

void UiEquipUpdatePlayerGauge(UiEquip *self, u8 player)
{
    s32 base;
    s32 i;
    float rect[4];
    float v;

    base = UiEquipGetPlayerSpriteBase(self, player);
    for (i = 0; i < 7; i++) {
        GfxSprite *sprite = ((GfxSprite **)self->base.data)[base + i];

        switch (i) {
        case 0:
            rect[0] = 0.0f;
            rect[1] = 0.0f;
            rect[2] = 240.0f;
            rect[3] = 272.0f;
            break;
        case 1:
            rect[0] = 0.0f;
            rect[1] = self->gaugeScroll[player].scrollFast;
            rect[2] = 32.0f;
            rect[3] = 272.0f;
            break;
        case 2:
            rect[0] = 32.0f;
            rect[1] = self->gaugeScroll[player].scrollFast;
            rect[2] = 32.0f;
            rect[3] = 272.0f;
            break;
        case 3:
            rect[0] = 0.0f;
            rect[1] = self->gaugeScroll[player].scrollFast;
            rect[2] = 64.0f;
            rect[3] = 272.0f;
            break;
        case 4:
            rect[0] = 64.0f;
            rect[1] = self->gaugeScroll[player].scrollFast;
            rect[2] = 64.0f;
            rect[3] = 272.0f;
            break;
        default: /* 5, 6 */
            rect[0] = 0.0f;
            rect[1] = self->gaugeScroll[player].scrollSlow;
            rect[2] = 240.0f;
            rect[3] = 272.0f;
            break;
        }
        GfxSpriteSetUvRectXYWH(sprite, rect);
    }

    v = self->gaugeScroll[player].scrollFast + 8.0f;
    self->gaugeScroll[player].scrollFast = v;
    if (!(v < 512.0f)) {
        self->gaugeScroll[player].scrollFast = 0.0f;
    }
    v = self->gaugeScroll[player].scrollSlow + 4.0f;
    self->gaugeScroll[player].scrollSlow = v;
    if (!(v < 512.0f)) {
        self->gaugeScroll[player].scrollSlow = 0.0f;
    }
}
