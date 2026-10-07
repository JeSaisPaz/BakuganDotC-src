// bdc 0x088d2810 UiFieldHudUpdateIcons
#include "bdc.h"

/* Layout step of the field HUD screen (task 3001, `UiFieldHudCtor`; sprite array `base.data`,
   player actor `player`) (called by its phase-2 update `UiFieldHudMainPhase`): copies the
   positions of a few frame sprites, offsets the map frame by a quarter of its size plus 90 and the
   player blip (sprite 1) by the player's world X/Z mapped through `mapRect`/`radarSize`/`radarHalf`,
   shows (flag bit 1) and shifts the icon groups gated by event flags 0x38d and 0x38e
   (`GameEventFlagTest`) and by `ActorPlayerCanThrow`, writes the profile's `fieldCounter` (one or
   two digits, `GfxSpriteSetCell` with 5 cells per row) into sprites 9/10, places the gauge sprites
   (shown only while `gaugeA`/`gaugeB` is exactly 0), refreshes the point counter
   (`UiFieldHudUpdateCounters`), shows guide sprites 0x48.. up to `iconBase + targetsDone`, and sets
   the alpha of sprites 0..0x47 to 0.4. */

#define HUD_SPRITE(i) (((GfxSprite **)self->base.data)[i])

/* Copies the 4-float position posX..posW (one lv.q/sv.q in the listing; bit copy). */
static void UiFieldHudCopyPos(GfxSprite *dst, const GfxSprite *src)
{
  float x = src->posX;
  float y = src->posY;
  float z = src->posZ;
  float w = src->posW;

  dst->posX = x;
  dst->posY = y;
  dst->posZ = z;
  dst->posW = w;
}

static void UiFieldHudShiftShow(UiFieldHud *self, s32 i)
{
  HUD_SPRITE(i)->posX += 90.0f;
  HUD_SPRITE(i)->posY += 90.0f;
  HUD_SPRITE(i)->flags |= 1;
}

static void UiFieldHudSetWhite(GfxSprite *sprite)
{
  sprite->tint[0] = 1.0f;
  sprite->tint[1] = 1.0f;
  sprite->tint[2] = 1.0f;
  sprite->alpha = 1.0f;
}

void UiFieldHudUpdateIcons(UiFieldHud *self)
{
  float quarterW;
  float quarterH;
  float frameOffX;
  float frameOffY;
  float blip;
  float base;
  s32 counter;
  s32 digit;
  s32 i;

  UiFieldHudCopyPos(HUD_SPRITE(0x45), HUD_SPRITE(0x26));
  UiFieldHudCopyPos(HUD_SPRITE(0x47), HUD_SPRITE(0x26));
  UiFieldHudCopyPos(HUD_SPRITE(0x46), HUD_SPRITE(0x27));

  quarterW = GfxSpriteGetWidth(HUD_SPRITE(0)) * 0.25f;
  quarterH = GfxSpriteGetHeight(HUD_SPRITE(0)) * 0.25f;
  frameOffX = quarterW + 90.0f;
  HUD_SPRITE(0)->posX -= frameOffX;
  HUD_SPRITE(0)->posY -= quarterH - 90.0f;
  HUD_SPRITE(0)->flags |= 1;

  /* player blip, X from world X */
  blip = (self->player->base.base.pos[0] - self->mapRect[2]) * self->radarSize[0] / self->mapRect[0] -
         self->radarHalf[0];
  base = HUD_SPRITE(0)->posX;
  base += GfxSpriteGetWidth(HUD_SPRITE(0)) * 0.5f;
  HUD_SPRITE(1)->posX = base + GfxSpriteGetWidth(HUD_SPRITE(1)) * 0.5f - blip;

  /* player blip, Y from world Z */
  blip = (self->player->base.base.pos[2] - self->mapRect[3]) * self->radarSize[1] / self->mapRect[1] -
         self->radarHalf[1];
  base = HUD_SPRITE(0)->posY;
  base += GfxSpriteGetHeight(HUD_SPRITE(0)) * 0.5f;
  HUD_SPRITE(1)->posY = base + GfxSpriteGetHeight(HUD_SPRITE(1)) * 0.5f - blip;

  HUD_SPRITE(0x43)->posX = HUD_SPRITE(0)->posX + 16.5f + 90.0f;
  HUD_SPRITE(0x43)->posY = HUD_SPRITE(0)->posY + 16.5f - 90.0f;

  GfxSpriteSetVCell(2.0f, HUD_SPRITE(0x2a));

  HUD_SPRITE(0x28)->flags |= 1;
  HUD_SPRITE(0x28)->posX -= 90.0f;
  HUD_SPRITE(0x28)->posY += 90.0f;
  HUD_SPRITE(0x1e)->flags |= 1;
  HUD_SPRITE(0x1e)->posX -= 90.0f;
  HUD_SPRITE(0x1e)->posY += 90.0f;

  if (GameEventFlagTest(0x38d)) {
    GfxSpriteCenterPivot(HUD_SPRITE(2));
    GfxSpriteResetMatrix(HUD_SPRITE(2));
    HUD_SPRITE(2)->flags |= 1;
    HUD_SPRITE(2)->posX += 90.0f;
    HUD_SPRITE(2)->posY += 90.0f;
    UiFieldHudShiftShow(self, 4);
    HUD_SPRITE(0x12)->posY += 90.0f;
    UiFieldHudSetWhite(HUD_SPRITE(0x12));
    GfxSpriteSetBottomCentrePivot(HUD_SPRITE(0x12));
    UiFieldHudShiftShow(self, 0x14);
    UiFieldHudShiftShow(self, 0x1c);
    GfxSpriteSetVCell(1.0f, HUD_SPRITE(0x1c));
  }

  if (GameEventFlagTest(0x38e)) {
    GfxSpriteCenterPivot(HUD_SPRITE(3));
    GfxSpriteResetMatrix(HUD_SPRITE(3));
    UiFieldHudShiftShow(self, 3);
    UiFieldHudShiftShow(self, 5);
    UiFieldHudSetWhite(HUD_SPRITE(0x11));
    GfxSpriteSetBottomCentrePivot(HUD_SPRITE(0x11));
    UiFieldHudShiftShow(self, 0x13);
    UiFieldHudShiftShow(self, 0x1b);
    GfxSpriteSetVCell(3.0f, HUD_SPRITE(0x1b));
  }

  if (ActorPlayerCanThrow(self->player)) {
    UiFieldHudShiftShow(self, 0x27);
    UiFieldHudShiftShow(self, 0x1d);
  }

  frameOffY = quarterH - 5.0f + 90.0f;
  for (i = 6; i <= 10; i++) {
    HUD_SPRITE(i)->posX -= frameOffX;
    HUD_SPRITE(i)->posY -= frameOffY;
    HUD_SPRITE(i)->flags |= 1;
  }

  counter = SaveGetProfile()->data->fieldCounter;
  if (counter >= 10) {
    digit = counter / 10;
    GfxSpriteSetCell(HUD_SPRITE(9), (float)(digit / 5), (float)(digit % 5));
    digit = counter % 10;
    GfxSpriteSetCell(HUD_SPRITE(10), (float)(digit / 5), (float)(digit % 5));
    HUD_SPRITE(10)->posX = HUD_SPRITE(9)->posX + 12.0f;
    HUD_SPRITE(9)->flags |= 1;
  } else {
    GfxSpriteSetCell(HUD_SPRITE(10), (float)(counter / 5), (float)(counter % 5));
    HUD_SPRITE(10)->posX = HUD_SPRITE(9)->posX + 6.0f - 2.0f;
    HUD_SPRITE(9)->flags &= ~1u;
  }

  base = HUD_SPRITE(0)->posX;
  HUD_SPRITE(0xd)->posX = base + GfxSpriteGetWidth(HUD_SPRITE(0)) * 0.5f + 90.0f;
  base = HUD_SPRITE(0)->posY;
  HUD_SPRITE(0xd)->posY = base + GfxSpriteGetHeight(HUD_SPRITE(0)) * 0.5f - 90.0f;
  HUD_SPRITE(0xd)->flags |= 1;

  HUD_SPRITE(0x15)->posY = HUD_SPRITE(4)->posY;
  HUD_SPRITE(0x15)->posX = HUD_SPRITE(5)->posX;
  if (self->player->gaugeA == 0.0f) {
    HUD_SPRITE(0x15)->flags |= 1;
  }
  HUD_SPRITE(0x44)->posY = HUD_SPRITE(4)->posY;
  HUD_SPRITE(0x44)->posX = HUD_SPRITE(4)->posX;
  if (self->player->gaugeB == 0.0f) {
    HUD_SPRITE(0x44)->flags |= 1;
  }

  HUD_SPRITE(0x2d)->posY = -48.0f;
  HUD_SPRITE(0x2e)->posY = -48.0f;
  UiFieldHudUpdateCounters(self);

  for (i = 0x36; i < 0x3e; i++) {
    UiFieldHudSetWhite(HUD_SPRITE(i));
    HUD_SPRITE(i)->posX -= 90.0f;
    HUD_SPRITE(i)->posY -= 90.0f;
  }
  HUD_SPRITE(0xe)->flags |= 1;

  for (i = 0x48; i < *(s32 *)self->iconBase + self->targetsDone + 0x48; i++) {
    HUD_SPRITE(i)->flags |= 1;
  }

  for (i = 0; i < 0x48; i++) {
    HUD_SPRITE(i)->alpha = 0.4f;
  }
}

#undef HUD_SPRITE
