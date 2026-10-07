// bdc 0x089294f4 UiHologramViewCreateSprites
#include "bdc.h"

/* Creates the 26 layout sprites (layout 0x3d, `UiLayoutCreateSprites`) of the hologram detail
   view (`UiHologramViewCtor`, task 392; view kind `+0x485`), hidden with alpha 0, centred pivots
   and unit scale; clears the sprite-state block `+0x74..+0x483`. */

void UiHologramViewCreateSprites(UiHologramView *self)
{
    GfxSprite **sprites;
    u32 i;

    memset(self->tweens, 0, 0x410);
    UiLayoutCreateSprites(self->base.spriteLayer, (GfxSprite **)self->base.data, 0x3d);
    sprites = (GfxSprite **)self->base.data;
    i = 0;
    do {
        sprites[i]->flags &= ~1u;
        sprites[i]->alpha = 0.0f;
        GfxSpriteCenterPivot(sprites[i]);
        UiSpriteSetScaleRotation(sprites[i], 1.0f, 1.0f, 0.0f);
        i++;
    } while (i < 0x1a);
}
