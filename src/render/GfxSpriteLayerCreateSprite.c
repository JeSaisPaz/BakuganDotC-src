// bdc 0x089f54a8 GfxSpriteLayerCreateSprite
#include "bdc.h"

/* Creates a screen sprite for `texture` on `layer`: takes a pool slot
   (`GfxSpriteLayerAllocPoolSlot` + `GfxSpriteInit` reset) or heap-allocates a fresh 0x160-byte
   `GfxSprite` (`GfxSpriteCtor` ctor); sets the texture, then for textures that are not
   power-of-two sized (`GfxTexture``.isPow2Size` clear) or when `smooth` selects quad mode 2
   (or 3 with linear filtering flag 0x20 when `smooth`) with full-texture UVs, else template mode 0;
   copies the vec4 `pos` to `posX..posW` and appends the sprite to the
   layer (`GfxSpriteLayerAdd`); returns the sprite (the value `GfxSpriteLayerAdd` returns). */

GfxSprite *GfxSpriteLayerCreateSprite(GfxSpriteLayer *self, void *texture, const float *pos, bool smooth)
{
  GfxSprite *sprite = NULL;
  GfxSprite *mem;
  bool fromLow;

  if (self->pool != NULL) {
    sprite = GfxSpriteLayerAllocPoolSlot(self);
    if (sprite != NULL) {
      GfxSpriteInit(sprite);
    }
  }
  if (sprite == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(GfxSprite), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    sprite = NULL;
    if (mem != NULL) {
      GfxSpriteCtor(mem);
      sprite = mem;
    }
  }
  sprite->texture = texture;
  if (((GfxTexture *)texture)->isPow2Size != 0 && !smooth) {
    GfxSpriteSetQuadMode(sprite, 0);
    sprite->billboardMode = 0;
  } else {
    if (smooth) {
      sprite->flags |= 0x20;
      GfxSpriteSetQuadMode(sprite, 3);
    } else {
      GfxSpriteSetQuadMode(sprite, 2);
    }
    GfxSpriteSetUvFull(sprite);
    sprite->billboardMode = 0;
  }
  sprite->posX = pos[0];
  sprite->posY = pos[1];
  sprite->posZ = pos[2];
  sprite->posW = pos[3];
  return (GfxSprite *)GfxSpriteLayerAdd(self, (CoreObject *)sprite);
}
