// bdc 0x08992954 UiUnlockCodeStartKeyPop
#include "bdc.h"

/* Starts the key-press "pop" of `UiUnlockCode` on key sprite `index`: clears the
   key's add colour, resets its matrix (`GfxSpriteResetMatrix`), copies it into the pop sprite
   (data `+0x84`, `GfxSpriteCopy`) and restarts the pop animation (`+0xa8` = 1, frame `+0xac` = 0)
   run by `UiUnlockCodeUpdateKeyPop`. */

void UiUnlockCodeStartKeyPop(UiUnlockCode *self, int index)
{
    GfxSprite **sprites = (GfxSprite **)self->base.data;
    GfxSprite *key = sprites[index];

    key->addColor[0] = 0.0f;
    key->addColor[1] = 0.0f;
    key->addColor[2] = 0.0f;
    key->addColor[3] = 1.0f;
    GfxSpriteResetMatrix(((GfxSprite **)self->base.data)[index]);
    sprites = (GfxSprite **)self->base.data;
    GfxSpriteCopy(sprites[index], sprites[0x84 / 4]);
    self->keyPopOn = 1;
    self->keyPopTimer = 0;
}
