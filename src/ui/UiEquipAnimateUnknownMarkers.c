// bdc 0x08965e20 UiEquipAnimateUnknownMarkers
#include "bdc.h"

/* Per-frame animation of the four "unknown" marker sprites (`spriteIdx[15]..+3`) of the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`), shown next to the current player's icon
   (`spriteIdx[9] + editPlayer`, fixed offsets per player and layout) while the grid cursor is on a
   Bakugan the player does not own (profile bit field `bakuganBitsA`) or on the random-pick button
   (`onRandom`). State byte `markerState`: 0 places the markers and starts them staggered by 10
   frames, 1 makes each one hop (half-cosine arc of 80 px over 40 frames, alpha dip) and spin
   (`UiEquipSpinMarker`), re-placing it at the next of five X offsets after each hop; when the
   cursor moves to an owned Bakugan they are hidden and the state resets. Returns at once while
   `markersHidden` is set (`UiEquipHideSelectionMarkers`) or in state 0 over an owned Bakugan.
   The two cosines are `vcos` of `angle * S703` (2/pi), i.e. the cosine of the angle in radians. */

/* Places marker sprite `idx` next to the current player's icon plus extra X offset `extraX`. */
static void UiEquipPlaceMarker(UiEquip *self, const s32 *offs, s32 idx, s32 extraX)
{
  GfxSprite **sprites;
  s32 player;

  sprites = (GfxSprite **)self->base.data;
  player = self->editPlayer;
  if (self->playerCount < 3) {
    sprites[idx]->posX = sprites[self->spriteIdx[9] + player]->posX + (float)offs[player * 2] +
                         (float)extraX;
    sprites = (GfxSprite **)self->base.data;
    sprites[idx]->posY = sprites[self->spriteIdx[9] + self->editPlayer]->posY +
                         (float)offs[self->editPlayer * 2 + 1];
  } else {
    sprites[idx]->posX = sprites[self->spriteIdx[9] + player]->posX + (float)offs[player * 2 + 4] +
                         (float)extraX;
    sprites = (GfxSprite **)self->base.data;
    sprites[idx]->posY = sprites[self->spriteIdx[9] + self->editPlayer]->posY +
                         (float)offs[self->editPlayer * 2 + 5];
  }
}

/* True when the grid cursor's Bakugan is not owned or the random-pick button is selected. */
static bool UiEquipCursorOnUnknown(UiEquip *self)
{
  SaveProfile *profile;
  s32 bakugan;
  u8 owned;

  profile = SaveGetProfile();
  bakugan = UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor);
  owned = profile->data->bakuganBitsA[bakugan / 8] & (1 << (bakugan % 8));
  return owned == 0 || self->onRandom != 0;
}

void UiEquipAnimateUnknownMarkers(UiEquip *self)
{
  /* [0..3] two-player X/Y per player, [4..11] four-player X/Y, [12..16] five extra X offsets */
  s32 offs[17] = {0x40, 8, -0x40, 8, 0x48, 0x1c, -0x48, 0x1c, 0x48, 0x1c, -0x48, 0x1c,
                  0, -0x14, 10, 0x14, -10};
  GfxSprite **sprites;
  UiTween *tween;
  s32 i;
  s32 idx;
  float c;

  if (self->markersHidden != 0) {
    return;
  }
  if (self->markerState == 0) {
    if (!UiEquipCursorOnUnknown(self)) {
      return;
    }
    for (i = 0; i < 4; i++) {
      idx = self->spriteIdx[15] + i;
      sprites = (GfxSprite **)self->base.data;
      GfxSpriteCenterPivot(sprites[idx]);
      sprites = (GfxSprite **)self->base.data;
      sprites[idx]->flags |= 0x20;
      sprites[idx]->blendMode = 2;
      sprites[idx]->alpha = 1.0f;
      UiSpriteSetScaleRotation(sprites[idx], 1.0f, 1.0f, 0.0f);
      UiEquipPlaceMarker(self, offs, idx, offs[12 + i]);
      tween = &self->tweens[idx];
      tween->t = 0.0f;
      tween->startAlpha = 1.0f;
      tween->cycle0c = (u8)i;
      tween->toggle07 = 0;
      tween->delay0b = (u8)(s32)((float)i * 10.0f);
      tween->slideEnd = (s16)(s32)((GfxSprite **)self->base.data)[idx]->posY;
    }
    self->markerState++;
  } else if (self->markerState < 2) {
    if (!UiEquipCursorOnUnknown(self)) {
      for (i = 0; i < 4; i++) {
        ((GfxSprite **)self->base.data)[self->spriteIdx[15] + i]->flags &= ~1u;
      }
      self->markerState = 0;
      return;
    }
    for (i = 0; i < 4; i++) {
      idx = self->spriteIdx[15] + i;
      tween = &self->tweens[idx];
      if (tween->delay0b != 0) {
        tween->delay0b--;
        continue;
      }
      UiEquipSpinMarker(self, idx);
      ((GfxSprite **)self->base.data)[idx]->flags |= 1;
      tween->t = tween->t + 0.025f;
      c = __builtin_cosf(tween->t * 3.1415927f);
      ((GfxSprite **)self->base.data)[idx]->alpha = tween->startAlpha - (1.0f - c) * 0.5f;
      c = __builtin_cosf(tween->t * 3.1415927f);
      ((GfxSprite **)self->base.data)[idx]->posY =
          (float)tween->slideEnd - (1.0f - c) * 0.5f * 80.0f;
      if (tween->t < 1.0f) {
        continue;
      }
      tween->cycle0c++;
      if (tween->cycle0c >= 5) {
        tween->cycle0c = 0;
      }
      UiEquipPlaceMarker(self, offs, idx, offs[12 + tween->cycle0c]);
      tween->t = 0.0f;
      tween->startAlpha = 1.0f;
      tween->slideEnd = (s16)(s32)((GfxSprite **)self->base.data)[idx]->posY;
    }
  }
}
