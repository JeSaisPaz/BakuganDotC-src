// bdc 0x0895ce68 UiEquipUpdatePlayerIconsTween
#include "bdc.h"

/* Advances the tween started by `UiEquipStartPlayerIconsTween` of the per-player icon sprites
   (sprites `spriteIdx[9]` .. `spriteIdx[9] + playerCount - 1`) on the UiEquip Bakugan/gear
   loadout screen (task 302, `UiEquipCtor`) (`UiTweenUpdate` flags 3, scale 1.5→1.0 in,
   1.0→1.5 out, 8 frames); returns true once any of the tweens has finished (they run in
   lockstep). */

bool UiEquipUpdatePlayerIconsTween(UiEquip *self, u8 out)
{
    GfxSprite **sprites;
    UiTween *tween;
    s32 i;
    u8 doneCount;

    doneCount = 0;
    i = self->spriteIdx[9];
    if (i < i + self->playerCount) {
        tween = &self->tweens[i];
        do {
            sprites = (GfxSprite **)self->base.data;
            if (out == 0) {
                doneCount += UiTweenUpdate(1.5f, 1.0f, 8.0f, out, sprites[i], tween, 3);
            } else {
                doneCount += UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprites[i], tween, 3);
            }
            i++;
            tween++;
        } while (i < self->spriteIdx[9] + self->playerCount);
    }
    return doneCount != 0;
}
