// bdc 0x0895cfa8 UiEquipStartPlayerLabelsTween
#include "bdc.h"

/* Starts the appear (`out` = 0) or disappear (`out` = 1) tween (`UiTweenBegin`, scale 1.5, mode
   3) of the per-player label/tab sprites (sprite range `+0x5170`, one per player) on the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`), clearing their visible bit first.
   The range start and player count are re-read every iteration. The original tests
   `editPlayer < i` but both arms clear the same bit, so that test is folded away. */

void UiEquipStartPlayerLabelsTween(UiEquip *self, u8 out)
{
    s32 first = self->spriteIdx[8];
    s32 i;
    GfxSprite *sprite;

    for (i = first; i < (s32)self->spriteIdx[8] + self->playerCount; i++) {
        sprite = ((GfxSprite **)self->base.data)[i];
        if (i - (s32)self->spriteIdx[8] < self->playerCount) {
            sprite->flags &= ~1u;
            sprite = ((GfxSprite **)self->base.data)[i];
        }
        UiTweenBegin(1.5f, out, sprite, &self->tweens[i], 3);
    }
}
