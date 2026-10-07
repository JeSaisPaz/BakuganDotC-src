// bdc 0x089b0004 UiBattleModeSelectCreateSprites
#include "bdc.h"

/* Builds the sprites of `UiBattleModeSelect`: creates 20 layout sprites
   (`UiLayoutCreateSprites` from the layout `+0x18` into `data`), clears the 0x500-byte animation
   state `+0x78`, enables both entries (`UiBattleModeSelectInitEntryEnabled`), sets the cursor
   `+0x74` to 0 and hides the first 12 sprites with alpha 0. */

void UiBattleModeSelectCreateSprites(UiBattleModeSelect *self)

{
  GfxSprite **sprites;
  int i;

  UiLayoutCreateSprites((self->base).spriteLayer,(self->base).data,0x14);
  memset(self->tweens,0,0x500);
  UiBattleModeSelectInitEntryEnabled(self);
  self->cursor = '\0';
  sprites = (GfxSprite **)(self->base).data;
  for (i = 0; i < 12; i++) {
    sprites[i]->flags &= ~1u;
    sprites[i]->alpha = 0.0f;
  }
}
