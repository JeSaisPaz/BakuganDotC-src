// bdc 0x089a4dc8 UiTitlePlateInit
#include "bdc.h"

/* Registers `sprite` as the shared screen-title plate (`g_uiTitlePlate`, 16 bytes of state reset)
   for the flip animation of `UiTitlePlateStep`: for the opening animation (`closing == 0`) it puts
   it on layer 2, makes it visible, centres the pivot, enables linear filtering, resets
   scale/rotation and centres it horizontally (x = 240); for the closing one it only resets
   scale/rotation. */

void UiTitlePlateInit(u8 closing, GfxSprite *sprite)
{
  memset(&g_uiTitlePlate, 0, sizeof(g_uiTitlePlate));
  g_uiTitlePlate.sprite = sprite;
  if (closing == 0) {
    sprite->layerMask = 2;
    g_uiTitlePlate.sprite->flags |= 1;
    GfxSpriteCenterPivot(g_uiTitlePlate.sprite);
    g_uiTitlePlate.sprite->flags |= 0x20;
    g_uiTitlePlate.sprite->scaleX = 1.0f;
    g_uiTitlePlate.sprite->scaleY = 1.0f;
    g_uiTitlePlate.sprite->angle = 0.0f;
    GfxSpriteSetScaleRotation(g_uiTitlePlate.sprite, g_uiTitlePlate.sprite->scaleX,
                              g_uiTitlePlate.sprite->scaleY, g_uiTitlePlate.sprite->angle, false);
    g_uiTitlePlate.sprite->posX = 240.0f;
  }
  else {
    sprite->scaleX = 1.0f;
    g_uiTitlePlate.sprite->scaleY = 1.0f;
    g_uiTitlePlate.sprite->angle = 0.0f;
    GfxSpriteSetScaleRotation(g_uiTitlePlate.sprite, g_uiTitlePlate.sprite->scaleX,
                              g_uiTitlePlate.sprite->scaleY, g_uiTitlePlate.sprite->angle, false);
  }
}
