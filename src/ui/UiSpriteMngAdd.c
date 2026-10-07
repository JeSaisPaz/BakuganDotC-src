// bdc 0x089ee480 UiSpriteMngAdd
#include "bdc.h"

/* Creates a sprite from a name (`GfxTryFindTexture(name)` then `UiSpriteCreate` with the manager's layer
   `+0x10`) and stores it in the first free of the 0x20 slots at `+0x14`, incrementing the count
   `+0x1c`. Returns the slot index or -1 (no layer, table full or creation failed; a sprite that
   found no slot is released with `UiSpriteLayerRelease`). Sprite handle returned to scripts by
   `ScriptOpSprite` cmd 1. */

int UiSpriteMngAdd(UiSpriteMng *self, const char *name, int x, int y, int z, int rx, int ry, int rw, int rh)

{
  void *texture;
  GfxSprite *sprite;
  int i;
  GfxSprite **slots;
  int slot;
  
  slot = -1;
  if ((((self->layer != (void *)0x0) && (self->count < 0x20)) &&
      (texture = GfxTryFindTexture(name), texture != (void *)0x0)) &&
     (sprite = UiSpriteCreate(self->layer,texture,rx,ry,rw,rh),
     sprite != (GfxSprite *)0x0)) {
    slots = self->sprites;
    i = 0;
    do {
      if (*slots == (GfxSprite *)0x0) {
        *slots = sprite;
        self->count = self->count + 1;
        slot = i;
        break;
      }
      i = i + 1;
      slots = slots + 1;
    } while (i < 0x20);
    if (slot < 0) {
      UiSpriteLayerRelease(self->layer,sprite);
    }
  }
  return slot;
}

