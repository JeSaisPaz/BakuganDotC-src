// bdc 0x089fece0 UiWindowFrameSetBorderTexture
#include "bdc.h"

/* Sets the border texture `tex` and state slot `slot` on the 8 border sprites of a 9-slice window
   frame (`UiWindowFrame`, 0x130 bytes: a `GfxSpriteLayer` with vtable
   `0x08af5954` at `+0x74`, plus a `CoreObject` at `+0x80`) and recomputes their UVs
   (`UiWindowFrameSetBorderUvs`). */

void UiWindowFrameSetBorderTexture(UiWindowFrame *self, void *tex, int slot)

{
  GfxSprite *sprite;
  int i;

  sprite = ((GfxSpriteLayer *)self)->head;
  self->borderTexture = tex;
  i = 0;
  do {
    sprite->texture = self->borderTexture;
    sprite->textureSlot = slot;
    i = i + 1;
    sprite = sprite->next;
  } while (i < 8);
  UiWindowFrameSetBorderUvs(self);
}
