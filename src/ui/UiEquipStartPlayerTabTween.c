// bdc 0x08962a18 UiEquipStartPlayerTabTween
#include "bdc.h"

/* Starts the zoom tween (`UiTweenBegin` scale 1.5, flags 3, `out` = fade out) of every per-player
   sprite group of the Bakugan/gear loadout screen before a battle (task 302, `UiEquipCtor`):
   player tabs (`spriteIdx[0x19]`, one per player), tinted tabs (`spriteIdx[0x1b]`, tint
   0.2/1.0/0.0), handicap stars (`spriteIdx[0x1d]`, `spriteIdx[0x1e]` per player, texture by
   `UiEquipSetHandicapStarTexture`: filled while `handicap[player] / (50*(star+1))` is non-zero),
   handicap buttons (`spriteIdx[0x41]`, `spriteIdx[0x42]` per player, icons 4/5 alternating) and
   handicap level cells (`spriteIdx[0x49]`, `spriteIdx[0x4a]` per player, cell row
   `handicap/(50*(n+1)) - 1`). Sets flags bit 0 on the sprites of players `0..editPlayer`, and for
   the buttons only on those of `editPlayer`. Every group's bounds are re-read from `self` each
   iteration. */

void UiEquipStartPlayerTabTween(UiEquip *self, u8 out)
{
  GfxSprite *sprite;
  UiTween *tween;
  int i;
  int k;
  int per;

  /* player tabs */
  tween = &self->tweens[self->spriteIdx[0x19]];
  for (i = self->spriteIdx[0x19]; i < self->spriteIdx[0x19] + self->playerCount; i++, tween++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if (i - self->spriteIdx[0x19] < self->editPlayer + 1) {
      sprite->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
    }
    UiTweenBegin(1.5f, out, sprite, tween, 3);
  }

  /* tinted tabs */
  tween = &self->tweens[self->spriteIdx[0x1b]];
  for (i = self->spriteIdx[0x1b]; i < self->spriteIdx[0x1b] + self->playerCount; i++, tween++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if (i - self->spriteIdx[0x1b] < self->editPlayer + 1) {
      sprite->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
    }
    /* the asm also rewrites alpha with its own value (a 4-float colour copy) */
    sprite->tint[0] = 0.2f;
    sprite->tint[1] = 1.0f;
    sprite->tint[2] = 0.0f;
    UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], tween, 3);
  }

  /* handicap stars */
  tween = &self->tweens[self->spriteIdx[0x1d]];
  for (i = self->spriteIdx[0x1d];
       i < self->spriteIdx[0x1d] + self->playerCount * self->spriteIdx[0x1e]; i++, tween++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if (i - self->spriteIdx[0x1d] < (self->editPlayer + 1) * self->spriteIdx[0x1e]) {
      sprite->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
    }
    k = i - self->spriteIdx[0x1d];
    per = self->spriteIdx[0x1e];
    UiEquipSetHandicapStarTexture(self, sprite,
                                  self->handicap[k / per] / ((k % per) * 50 + 50) != 0);
    UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], tween, 3);
  }

  /* handicap buttons */
  tween = &self->tweens[self->spriteIdx[0x41]];
  for (i = self->spriteIdx[0x41];
       i < self->spriteIdx[0x41] + self->playerCount * self->spriteIdx[0x42]; i++, tween++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if ((i - self->spriteIdx[0x41]) / self->spriteIdx[0x42] == self->editPlayer) {
      sprite->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
    }
    if (((i - self->spriteIdx[0x41]) & 1) == 0)
      UiSetButtonIcon(sprite, 4);
    else
      UiSetButtonIcon(sprite, 5);
    UiTweenBegin(1.5f, out, ((GfxSprite **)self->base.data)[i], tween, 3);
  }

  /* handicap level cells */
  tween = &self->tweens[self->spriteIdx[0x49]];
  for (i = self->spriteIdx[0x49];
       i < self->spriteIdx[0x49] + self->playerCount * self->spriteIdx[0x4a]; i++, tween++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if (!(self->editPlayer < (i - self->spriteIdx[0x49]) / self->spriteIdx[0x4a])) {
      sprite->flags |= 1;
      k = i - self->spriteIdx[0x49];
      per = self->spriteIdx[0x4a];
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], 0.0f,
                       (float)(u8)(self->handicap[k / per] / ((k % per) * 50 + 50) - 1));
      sprite = ((GfxSprite **)self->base.data)[i];
    }
    UiTweenBegin(1.5f, out, sprite, tween, 3);
  }
}
