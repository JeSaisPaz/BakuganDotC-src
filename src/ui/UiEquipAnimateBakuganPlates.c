// bdc 0x08959ffc UiEquipAnimateBakuganPlates
#include "bdc.h"

/* Animates the per-player Bakugan plates of `UiEquip` (the Bakugan/gear loadout
   screen before a battle). For each of the 4 players whose stamp record `stampRec[p]` is active
   (`+0`), by state `+1`:
   0: next frame go to 1 if a Bakugan id (`+2`) is set, else to 3;
   1: shows the plate sprite markerBase[p] (cell row = attribute `+3`), the name sprite
      markerSpriteA[p] (`UiBakuganSetNameTexture` for the id) and the attribute icon
      markerSpriteB[p] (cell `+3` of a 3-column sheet), each centred (`GfxSpriteCenterPivot`) at
      1.2x (`UiSpriteSetScaleRotation`); the plate starts transparent 64 px left (even player) or
      right (odd player, mirrored with `GfxSpriteFlipU`) of markerBasePos[p]; then state 2;
   2: pop-in over 8 frames (t += 0.125, ease 1 - (t-1)^2): alpha up by 1, scale down by 0.2, X
      slides 64 px to rest; at t >= 1 snaps alpha/scale to 1 and X to rest, state 5;
   3: snapshots the plate's alpha, scale and X, state 4;
   4: fade-out over 8 frames (ease t^2): alpha down by 1, scale up by 0.2, X slides 64 px outward;
      at t >= 1 hides the three sprites (flags bit 0), state 5;
   >= 5: deactivates the record.
   In states 2 and 4 the name and icon sprites copy the plate's alpha and scale and all three
   rebuild their matrices (`GfxSpriteSetScaleRotation`). */

static GfxSprite *PlateSprite(UiEquip *self, u8 idx)
{
  return ((GfxSprite **)self->base.data)[idx];
}

static void PlateApplyTransform(GfxSprite *sp)
{
  GfxSpriteSetScaleRotation(sp, sp->scaleX, sp->scaleY, sp->angle, false);
}

/* Copies the plate's alpha/scale to the name (A) and icon (B) sprites and rebuilds all three. */
static void PlateSyncSprites(UiEquip *self, int i)
{
  GfxSprite *sp;

  sp = PlateSprite(self, self->markerBase[i]);
  sp->scaleY = sp->scaleX;
  PlateApplyTransform(PlateSprite(self, self->markerBase[i]));

  PlateSprite(self, self->markerSpriteA[i])->alpha = PlateSprite(self, self->markerBase[i])->alpha;
  PlateSprite(self, self->markerSpriteA[i])->scaleX = PlateSprite(self, self->markerBase[i])->scaleX;
  PlateSprite(self, self->markerSpriteA[i])->scaleY = PlateSprite(self, self->markerBase[i])->scaleY;
  PlateApplyTransform(PlateSprite(self, self->markerSpriteA[i]));

  PlateSprite(self, self->markerSpriteB[i])->alpha = PlateSprite(self, self->markerBase[i])->alpha;
  PlateSprite(self, self->markerSpriteB[i])->scaleX = PlateSprite(self, self->markerBase[i])->scaleX;
  PlateSprite(self, self->markerSpriteB[i])->scaleY = PlateSprite(self, self->markerBase[i])->scaleY;
  PlateApplyTransform(PlateSprite(self, self->markerSpriteB[i]));
}

/* Centres sprite `idx`, makes it visible and sets it to 1.2x. */
static void PlatePopSprite(UiEquip *self, u8 *idx)
{
  PlateSprite(self, *idx)->flags |= 1;
  GfxSpriteCenterPivot(PlateSprite(self, *idx));
  PlateSprite(self, *idx)->flags |= 0x20;
  UiSpriteSetScaleRotation(PlateSprite(self, *idx), 1.2f, 1.2f, 0.0f);
}

void UiEquipAnimateBakuganPlates(UiEquip *self)
{
  int i;
  u8 *rec;
  u8 state;
  UiTween *tw;
  GfxSprite *sp;
  float t;
  float ease;

  for (i = 0; i < 4; i++) {
    rec = self->stampRec[i];
    if (rec[0] == 0) {
      continue;
    }
    state = rec[1];
    if (state >= 5) {
      rec[0] = 0;
      continue;
    }
    switch (state) {
    case 1:
      GfxSpriteSetCell(PlateSprite(self, self->markerBase[i]), 0.0f, (float)rec[3]);
      PlatePopSprite(self, &self->markerBase[i]);
      self->tweens[self->markerBase[i]].t = 0.0f;
      PlateSprite(self, self->markerBase[i])->alpha = 0.0f;
      self->tweens[self->markerBase[i]].startAlpha = 0.0f;
      self->tweens[self->markerBase[i]].startScale = PlateSprite(self, self->markerBase[i])->scaleX;
      sp = PlateSprite(self, self->markerBase[i]);
      if ((i & 1) == 0) {
        sp->posX = self->markerBasePos[i][0] - 64.0f;
      } else {
        sp->posX = self->markerBasePos[i][0] + 64.0f;
        GfxSpriteFlipU(PlateSprite(self, self->markerBase[i]));
      }
      tw = &self->tweens[self->markerBase[i]];
      sp = PlateSprite(self, self->markerBase[i]);
      tw->plateRestX = self->markerBasePos[i][0];
      tw->plateFromX = sp->posX;
      UiBakuganSetNameTexture(PlateSprite(self, self->markerSpriteA[i]), rec[2]);
      PlatePopSprite(self, &self->markerSpriteA[i]);
      GfxSpriteSetCell(PlateSprite(self, self->markerSpriteB[i]), (float)(rec[3] / 3),
                       (float)(rec[3] % 3));
      PlatePopSprite(self, &self->markerSpriteB[i]);
      rec[1] = rec[1] + 1;
      break;

    case 2:
      tw = &self->tweens[self->markerBase[i]];
      t = tw->t + 0.125f;
      tw->t = t;
      PlateSprite(self, self->markerBase[i])->alpha = tw->startAlpha + (1.0f - (t - 1.0f) * (t - 1.0f));
      tw = &self->tweens[self->markerBase[i]];
      t = tw->t - 1.0f;
      PlateSprite(self, self->markerBase[i])->scaleX = tw->startScale - (1.0f - t * t) * 0.20000005f;
      tw = &self->tweens[self->markerBase[i]];
      sp = PlateSprite(self, self->markerBase[i]);
      t = tw->t - 1.0f;
      ease = (1.0f - t * t) * 64.0f;
      if ((i & 1) == 0) {
        sp->posX = tw->plateFromX + ease;
      } else {
        sp->posX = tw->plateFromX - ease;
      }
      sp = PlateSprite(self, self->markerBase[i]);
      if (!(self->tweens[self->markerBase[i]].t < 1.0f)) {
        sp->alpha = 1.0f;
        PlateSprite(self, self->markerBase[i])->scaleX = 1.0f;
        PlateSprite(self, self->markerBase[i])->posX = self->markerBasePos[i][0];
        rec[1] = 5;
      }
      PlateSyncSprites(self, i);
      break;

    case 3:
      tw = &self->tweens[self->markerBase[i]];
      sp = PlateSprite(self, self->markerBase[i]);
      tw->t = 0.0f;
      tw->startAlpha = sp->alpha;
      tw->startScale = sp->scaleX;
      tw->plateFromX = sp->posX;
      tw->plateRestX = self->markerBasePos[i][0];
      rec[1] = state + 1;
      break;

    case 4:
      tw = &self->tweens[self->markerBase[i]];
      t = tw->t + 0.125f;
      tw->t = t;
      PlateSprite(self, self->markerBase[i])->alpha = tw->startAlpha - t * t;
      tw = &self->tweens[self->markerBase[i]];
      t = tw->t;
      PlateSprite(self, self->markerBase[i])->scaleX = tw->startScale + t * t * 0.20000005f;
      tw = &self->tweens[self->markerBase[i]];
      sp = PlateSprite(self, self->markerBase[i]);
      t = tw->t;
      ease = t * t * 64.0f;
      if ((i & 1) == 0) {
        sp->posX = tw->plateFromX - ease;
      } else {
        sp->posX = tw->plateFromX + ease;
      }
      sp = PlateSprite(self, self->markerBase[i]);
      if (!(self->tweens[self->markerBase[i]].t < 1.0f)) {
        sp->flags &= ~1u;
        PlateSprite(self, self->markerSpriteA[i])->flags &= ~1u;
        PlateSprite(self, self->markerSpriteB[i])->flags &= ~1u;
        rec[1] = 5;
      }
      PlateSyncSprites(self, i);
      break;

    default: /* 0 */
      if (rec[2] != 0) {
        rec[1] = state + 1;
      } else {
        rec[1] = 3;
      }
      break;
    }
  }
}
