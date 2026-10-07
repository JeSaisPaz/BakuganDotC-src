// bdc 0x0895eecc UiEquipSpinMarker
#include "bdc.h"

/* Steps a fake Y-axis spin of sprite `idx` of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): shrinks `scaleX` by 0.1 per frame down to 0.1, mirrors the sprite
   (`GfxSpriteFlipU`) and grows it back to 1, then repeats; the direction is byte `+7`
   (`toggle07`) of the sprite's tween record (`+0x78 + idx*0x28`). Applies the scale with
   `UiSpriteSetScaleRotation`.
    */

void UiEquipSpinMarker(UiEquip *self, s32 idx)
{
    GfxSprite *sprite;

    sprite = ((GfxSprite **)self->base.data)[idx];
    if (self->tweens[idx].toggle07 == 0) {
        sprite->scaleX = sprite->scaleX - 0.1f;
        sprite = ((GfxSprite **)self->base.data)[idx];
        if (sprite->scaleX <= 0.1f) {
            GfxSpriteFlipU(sprite);
            self->tweens[idx].toggle07 = 1;
        }
    } else {
        sprite->scaleX = sprite->scaleX + 0.1f;
        sprite = ((GfxSprite **)self->base.data)[idx];
        if (!(sprite->scaleX < 1.0f)) {
            self->tweens[idx].toggle07 = 0;
        }
    }
    sprite = ((GfxSprite **)self->base.data)[idx];
    UiSpriteSetScaleRotation(sprite, sprite->scaleX, 1.0f, 0.0f);
}
