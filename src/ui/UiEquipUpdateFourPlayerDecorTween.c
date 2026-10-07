// bdc 0x0895f110 UiEquipUpdateFourPlayerDecorTween
#include "bdc.h"

/* Advances the tween started by `UiEquipStartFourPlayerDecorTween` of the three decoration
   sprites of the four-player layout (three sprites from `spriteIdx[0x10]`, `+0x5180`) on the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`) (`UiTweenUpdate`, 1.5→1.0 in,
   1.0→1.5 out, 8 frames). Returns true with fewer than three players; otherwise returns true
   when at least one of the three tweens has finished (the u8 sum of the results is non-zero). */

bool UiEquipUpdateFourPlayerDecorTween(UiEquip *self, u8 out)
{
    GfxSprite **sprites;
    int i;
    u8 sum;

    if (self->playerCount < 3) {
        return true;
    }
    sum = 0;
    for (i = self->spriteIdx[0x10]; i < self->spriteIdx[0x10] + 3; i++) {
        sprites = (GfxSprite **)self->base.data;
        if (out == 0) {
            sum += UiTweenUpdate(1.5f, 1.0f, 8.0f, out, sprites[i], &self->tweens[i], 1);
        } else {
            sum += UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprites[i], &self->tweens[i], 1);
        }
    }
    return sum != 0;
}
