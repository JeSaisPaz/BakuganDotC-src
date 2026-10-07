// bdc 0x0894e4b0 UiNetMenuStartButtonSlide
#include "bdc.h"

/* Starts the slide of the two arrow sprites (2–3) of `UiNetMenu` (records
   `arrowSlide[0..1]`): opening makes them visible, centred, linearly filtered at scale 1, keeps the
   layout X as the slide target, moves them to X = 240 (start X) and stores the distance
   (`UiAbsDiff`) and start alpha; closing records the current alpha, X scale and X as start values.
   Both reset the tween time. */

void UiNetMenuStartButtonSlide(UiScreen *screen, u8 closing)
{
  UiNetMenu *menu = (UiNetMenu *)screen;
  UiNetMenuButtonSlide *slide;
  GfxSprite **sprites;
  GfxSprite *sprite;
  s32 i;
  float dist;

  if (closing == 0) {
    for (i = 2; i < 4; i++) {
      slide = &menu->arrowSlide[i - 2];
      ((GfxSprite **)screen->data)[i]->flags |= 1;
      GfxSpriteCenterPivot(((GfxSprite **)screen->data)[i]);
      ((GfxSprite **)screen->data)[i]->flags |= 0x20;
      UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
      sprite = ((GfxSprite **)screen->data)[i];
      slide->toX = (s16)(s32)sprite->posX;
      sprite->posX = 240.0f;
      sprite = ((GfxSprite **)screen->data)[i];
      slide->fromX = (s16)(s32)sprite->posX;
      dist = UiAbsDiff((float)slide->toX, sprite->posX);
      slide->t = 0.0f;
      slide->distance = (s16)(s32)dist;
      slide->startAlpha = ((GfxSprite **)screen->data)[i]->alpha;
    }
  } else {
    sprites = (GfxSprite **)screen->data;
    for (i = 2; i < 4; i++) {
      slide = &menu->arrowSlide[i - 2];
      slide->t = 0.0f;
      slide->startAlpha = sprites[i]->alpha;
      slide->startScale = sprites[i]->scaleX;
      slide->fromX = (s16)(s32)sprites[i]->posX;
    }
  }
}
