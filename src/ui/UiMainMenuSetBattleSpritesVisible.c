// bdc 0x089a7520 UiMainMenuSetBattleSpritesVisible
#include "bdc.h"

/* Shows (`show` 1) or hides the two battle-entry sprites `data+0xc` and `data+0x10`. */

void UiMainMenuSetBattleSpritesVisible(UiMainMenu *self, u8 show)

{
  GfxSprite **sprites = (GfxSprite **)(self->base).data;
  GfxSprite *sprite = sprites[3];

  if (show != 0) {
    sprite->flags |= 1;
    sprite = ((GfxSprite **)(self->base).data)[4];
    sprite->flags |= 1;
    return;
  }
  sprite->flags &= ~1u;
  sprite = ((GfxSprite **)(self->base).data)[4];
  sprite->flags &= ~1u;
}
