// bdc 0x08999964 UiWorldMapJetRingsStep
#include "bdc.h"

/* Ring effect around Marucho's jet on `UiWorldMap` (sprites 0x1c..0x1f, tweens
   `spriteTween[0x1c..0x1f]`): on the first call (`jetRingStep` = 0) places the four rings at the
   jet's screen position (`jet.x * 1.8 + 130`, `148 − jet.y * 1.5`), scale 0 at depth 10 + i (the
   fourth at scale 1, depth 0, one pixel higher) with staggered delays of 8 frames (hidden when the
   rank-mode globe motion is 2) and moves to step 1; at step 1 expands the first three by up to 1.4
   while fading them out over 50 frames, keeps all four on the jet, and goes back to step 0 once all
   three are done. Any other step does nothing. */

void UiWorldMapJetRingsStep(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite *spr;
  UiTween *tw;
  float d;
  u8 done;
  int i;

  if (map->jetRingStep == 0) {
    for (i = 0; i < 4; i++) {
      tw = &map->spriteTween[0x1c + i];
      ((GfxSprite **)screen->data)[0x1c + i]->layerMask = 2;
      ((GfxSprite **)screen->data)[0x1c + i]->flags |= 1;
      if (UiWorldMapIsRankMode(screen) == true && UiWorldMapGetGlobeMotion(screen) == 2) {
        ((GfxSprite **)screen->data)[0x1c + i]->flags &= ~1u;
      }
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
      spr->posX = map->jetModel->pos[0] * 1.8f + 130.0f;
      spr->posY = (136.0f - map->jetModel->pos[1] * 1.5f) + 12.0f;
      if (i == 3) {
        spr->posY = spr->posY - 1.0f;
      }
      tw->t = 0.0f;
      tw->startAlpha = spr->alpha;
      tw->startScale = spr->scaleX;
      tw->delay0b = (u8)(int)((float)i * 8.0f);
    }
    map->jetRingStep++;
  } else if (map->jetRingStep < 2) {
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
    for (i = 0; i < 4; i++) {
      spr = ((GfxSprite **)screen->data)[0x1c + i];
      spr->posX = map->jetModel->pos[0] * 1.8f + 130.0f;
      spr->posY = (136.0f - map->jetModel->pos[1] * 1.5f) + 12.0f;
      if (i == 3) {
        spr->posY = spr->posY - 1.0f;
      }
    }
    if (done == 3) {
      map->jetRingStep = 0;
    }
  }
}
