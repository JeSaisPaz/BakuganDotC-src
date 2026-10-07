// bdc 0x0895ecf8 UiEquipStartPlayerDoneMarkTween
#include "bdc.h"

/* Starts the appear (`out` = 0, made visible) or disappear tween (`UiTweenBegin` 1.5, mode 3) of
   player `player`'s "done" mark (sprite `+0x5174 + player`) on the UiEquip Bakugan/gear loadout
   screen (task 302, `UiEquipCtor`). */

void UiEquipStartPlayerDoneMarkTween(UiEquip *self, u8 out, u8 player)

{
  GfxSprite *sprite;
  s32 idx;

  idx = self->spriteIdx[10] + player;
  sprite = ((GfxSprite **)self->base.data)[idx];
  if (out == 0) {
    sprite->flags = sprite->flags | 1;
    sprite = ((GfxSprite **)self->base.data)[idx];
  } else {
    sprite->flags = sprite->flags & ~1u;
    sprite = ((GfxSprite **)self->base.data)[idx];
  }
  UiTweenBegin(1.5f, out, sprite, &self->tweens[idx], 3);
}
