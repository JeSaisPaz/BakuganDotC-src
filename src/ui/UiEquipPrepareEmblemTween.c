// bdc 0x0895ccd8 UiEquipPrepareEmblemTween
#include "bdc.h"

/* Prepares the emblem's `UiTween` record (`+0x78 + idx*0x28`, idx = `+0x5176`) before
   `UiEquipUpdateEmblemTween` on the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): resets the progress `t` to 0 and snapshots the sprite's `scaleX` into
   `startScale`; for `out` also its `alpha` into `startAlpha`. */

void UiEquipPrepareEmblemTween(UiEquip *self, bool out)

{
  GfxSprite **sprites;
  uint idx;

  idx = self->spriteIdx[0xb];
  sprites = (GfxSprite **)(self->base).data;
  if (!out) {
    self->tweens[idx].t = 0.0f;
    self->tweens[idx].startScale = sprites[idx]->scaleX;
    return;
  }
  self->tweens[idx].t = 0.0f;
  self->tweens[idx].startScale = sprites[idx]->scaleX;
  self->tweens[idx].startAlpha = sprites[idx]->alpha;
}
