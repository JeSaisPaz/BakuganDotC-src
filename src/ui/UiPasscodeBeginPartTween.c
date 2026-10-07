// bdc 0x0893dbf8 UiPasscodeBeginPartTween
#include "bdc.h"

/* Initialises the tween record of sprite `index` of `UiPasscode` (`tween[index]`:
   t = 0, start alpha, start scale, flag cleared); when opening first centres the pivot, enables
   linear filtering (flag 0x20) and sets scale 0. */

void UiPasscodeBeginPartTween(UiScreen *screen, u8 closing, u8 index)
{
  UiPasscodePartTween *tw = &((UiPasscode *)screen)->tween[index];
  GfxSprite **sprites = (GfxSprite **)screen->data;

  if (closing == 0) {
    GfxSpriteCenterPivot(sprites[index]);
    sprites[index]->flags |= 0x20;
    UiSpriteSetScaleRotation(sprites[index], 0.0f, 0.0f, 0.0f);
  }
  tw->t = 0.0f;
  tw->startAlpha = sprites[index]->alpha;
  tw->flag = 0;
  tw->startScale = sprites[index]->scaleX;
}
