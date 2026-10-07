// bdc 0x089f5c50 GfxSpriteLayerSetFlagsAll
#include "bdc.h"

/* ORs `flags` into the `flags` word (`+0xd0`) of every sprite in the layer's draw list. */

void GfxSpriteLayerSetFlagsAll(GfxSpriteLayer *self, u32 flags)
{
  GfxSprite *sprite;

  for (sprite = self->head; sprite != NULL; sprite = sprite->next) {
    sprite->flags |= flags;
  }
}
