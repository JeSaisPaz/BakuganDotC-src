// bdc 0x089f5704 GfxSpriteLayerCreateBillboard
#include "bdc.h"

/* Creates a 3D billboard sprite for `texture` on `layer`: takes a pool slot (reusing an already
   linked one when its `slotFlags` bit 4 is set) or heap-allocates one, resets the UV offset/scale
   (`uvOffsetU..uvScaleV` = 0, 0, 1, 1), sets the texture and the linear filter flag 0x20, selects
   quad mode 3 with full UVs (or template mode 1 for power-of-two textures, `GfxTexture``.isPow2Size`),
   sets `billboardMode` 0, zeroes `maybe_billboardParams80` (bank constant C720 = 0)
   and, unless the slot was already linked, appends the sprite (`GfxSpriteLayerAdd`) and returns
   what that returns (the sprite); an already linked slot is returned directly. */

GfxSprite *GfxSpriteLayerCreateBillboard(GfxSpriteLayer *self, void *texture)
{
  GfxSprite *sprite = NULL;
  GfxSprite *mem;
  bool linked = false;
  bool fromLow;

  if (self->pool != NULL) {
    sprite = GfxSpriteLayerAllocPoolSlot(self);
    if (sprite != NULL && (sprite->slotFlags & 4) != 0) {
      linked = true;
    }
  }
  if (sprite == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(0x160, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    sprite = NULL;
    if (mem != NULL) {
      GfxSpriteCtor(mem);
      sprite = mem;
    }
  }
  sprite->uvOffsetU = 0.0f;
  sprite->uvOffsetV = 0.0f;
  sprite->uvScaleU = 1.0f;
  sprite->uvScaleV = 1.0f;
  sprite->texture = texture;
  sprite->flags |= 0x20;
  if (((GfxTexture *)texture)->isPow2Size != 0) {
    GfxSpriteSetQuadMode(sprite, 1);
    sprite->billboardMode = 0;
  } else {
    GfxSpriteSetQuadMode(sprite, 3);
    GfxSpriteSetUvFull(sprite);
    sprite->billboardMode = 0;
  }
  sprite->maybe_billboardParams80[0] = 0.0f;
  sprite->maybe_billboardParams80[1] = 0.0f;
  sprite->maybe_billboardParams80[2] = 0.0f;
  sprite->maybe_billboardParams80[3] = 0.0f;
  if (linked) {
    return sprite;
  }
  return (GfxSprite *)GfxSpriteLayerAdd(self, (CoreObject *)sprite);
}
