// bdc 0x08999e5c UiWorldMapUpdateGlobe
#include "bdc.h"

/* Per-frame globe/jet update of `UiWorldMap` once the map model is fully shown
   (`mapModel->ambient[3]` = 1; else hides the ring sprites 0x1c..0x1f and returns): when the
   selected area changes (`areaId` vs `shownAreaId`) starts a 40-frame turn
   (`UiWorldMapStartGlobeTurn`), picks the mode `globeMotion` from `UiWorldMapGetGlobeMotion`
   (0 → 2 turn, 1 → 3 idle spin, 2 → 4 spin with jet rings, after resetting `jetRingStep`) and hides
   the rings; then runs the mode: 0 re-arms the four rings and moves to 1, 1 expands/fades the first
   three rings and goes back to 0 once all three are done, 2 `UiWorldMapGlobeTurnStep` (back to 0
   when it reports done), 3 `UiWorldMapIdleSpin`, 4 spin + `UiWorldMapJetRingsStep`. Hides the
   rings when `ringsHidden` is set, eases `jetScale` towards 0.8 (mode 4) or 0.6 (otherwise) while
   `jetActive`, and applies the rotation (`UiWorldMapApplyGlobeRotation`). */

void UiWorldMapUpdateGlobe(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite *spr;
  UiTween *tw;
  float d;
  u8 motion;
  u8 done;
  int i;

  if (map->mapModel->ambient[3] != 1.0f) {
    for (i = 0; i < 4; i++) {
      ((GfxSprite **)screen->data)[0x1c + i]->flags &= ~1u;
    }
    return;
  }
  if (map->areaId != map->shownAreaId) {
    map->shownAreaId = map->areaId;
    UiWorldMapStartGlobeTurn(40.0f, screen);
    motion = (u8)UiWorldMapGetGlobeMotion(screen);
    if (motion == 0) {
      map->globeMotion = 2;
    } else if (motion < 2) {
      map->globeMotion = 3;
    } else if (motion < 3) {
      map->jetRingStep = 0;
      map->globeMotion = 4;
    }
    for (i = 0; i < 4; i++) {
      ((GfxSprite **)screen->data)[0x1c + i]->flags &= ~1u;
    }
  }
  switch (map->globeMotion) {
  case 0:
    for (i = 0; i < 4; i++) {
      tw = &map->spriteTween[0x1c + i];
      ((GfxSprite **)screen->data)[0x1c + i]->layerMask = 1;
      ((GfxSprite **)screen->data)[0x1c + i]->flags |= 1;
      GfxSpriteCenterPivot(((GfxSprite **)screen->data)[0x1c + i]);
      if (i == 3) {
        UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[0x1c + i], 1.0f, 1.0f, 1.0f);
        ((GfxSprite **)screen->data)[0x1c + i]->posZ = 0.0f;
      } else {
        UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[0x1c + i], 0.0f, 0.0f, 0.0f);
        ((GfxSprite **)screen->data)[0x1c + i]->posZ = (float)(i + 10);
      }
      spr = ((GfxSprite **)screen->data)[0x1c + i];
      spr->alpha = 1.0f;
      spr->posX = 130.0f;
      spr->posY = 136.0f;
      if (i == 3) {
        spr->posY = spr->posY - 1.0f;
      }
      tw->t = 0.0f;
      tw->startAlpha = spr->alpha;
      tw->startScale = spr->scaleX;
      tw->delay0b = (u8)(int)((float)i * 8.0f);
    }
    map->globeMotion++;
    break;
  case 1:
    done = 0;
    for (i = 0; i < 3; i++) {
      tw = &map->spriteTween[0x1c + i];
      if (tw->delay0b != 0) {
        tw->delay0b--;
        continue;
      }
      tw->t = tw->t + 0.02f;
      d = tw->t - 1.0f;
      spr = ((GfxSprite **)screen->data)[0x1c + i];
      spr->alpha = tw->startAlpha - (1.0f - d * d);
      d = tw->t - 1.0f;
      spr->scaleX = tw->startScale + (1.0f - d * d) * 1.4f;
      spr->scaleY = spr->scaleX;
      GfxSpriteSetScaleRotation(spr, spr->scaleX, spr->scaleY, spr->angle, false);
      if (!(tw->t < 1.0f)) {
        done++;
        ((GfxSprite **)screen->data)[0x1c + i]->alpha = 0.0f;
      }
    }
    if (done == 3) {
      map->globeMotion = 0;
    }
    break;
  case 2:
    if (UiWorldMapGlobeTurnStep(screen) != 0) {
      map->globeMotion = 0;
    }
    break;
  case 3:
    UiWorldMapIdleSpin(screen);
    break;
  case 4:
    UiWorldMapIdleSpin(screen);
    UiWorldMapJetRingsStep(screen);
    break;
  }
  if (map->ringsHidden != 0) {
    for (i = 0; i < 4; i++) {
      ((GfxSprite **)screen->data)[0x1c + i]->flags &= ~1u;
    }
  }
  if (map->jetActive != 0) {
    if (map->globeMotion == 4) {
      map->jetScale = map->jetScale + 0.02f;
      if (!(map->jetScale <= 0.8f)) {
        map->jetScale = 0.8f;
      }
    } else {
      map->jetScale = map->jetScale - 0.02f;
      if (map->jetScale < 0.6f) {
        map->jetScale = 0.6f;
      }
    }
  }
  UiWorldMapApplyGlobeRotation(screen);
}
