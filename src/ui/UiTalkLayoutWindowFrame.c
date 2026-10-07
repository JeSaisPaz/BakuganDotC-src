// bdc 0x08834df8 UiTalkLayoutWindowFrame
#include "bdc.h"

/* Lays out the frame of the battle talk window (task 0x6e, `UiGetTalkTask`) around the current
   text: the vertical scale is 0.5, 0.75 or 1.0 for 1, 2 or more text lines (text height
   `talkTextHeight` against the printer's `lineHeight`, printer `overlayObj[0]`); the left cap
   (sprite 0x7e), the stretched middle (0x7f, width from `talkTextWidth`) and the right cap (0x80)
   are centred on the 384-px area (left edge clamped to 0), and the speaker tail/portrait sprites
   (0x81..0x83) are placed from the left cap or, for narrow windows (`UiTalkUsesNarrowIndent`),
   from the right cap with sprites 0x82–0x83 mirrored (`GfxSpriteFlipU`). Does nothing past the
   scale pick when there is no printer (the line height is read before that check). */

void UiTalkLayoutWindowFrame(void *win)
{
  BtlHud *hud = (BtlHud *)win;
  UiTextPrinter *printer;
  GfxSprite *sprite;
  float scale;
  float left;
  float frameW;
  float capX;
  float middleW;
  float midX;
  float t;
  s32 kind;
  s32 i;

  printer = (UiTextPrinter *)hud->overlayObj[0];
  if (hud->talkTextHeight <= printer->lineHeight) {
    scale = 0.5f;
  }
  else if (hud->talkTextHeight <= printer->lineHeight * 2.0f) {
    scale = 0.75f;
  }
  else {
    scale = 1.0f;
  }
  if (printer == NULL) {
    return;
  }

  left = ((384.0f - hud->talkTextWidth) - 64.0f) * 0.5f;
  frameW = (float)(s32)((hud->talkTextWidth + 40.0f) * 320.0f * 0.0026041667f + 64.0f);
  if (left < 0.0f) {
    left = 0.0f;
    frameW = 336.0f;
  }
  capX = left + 16.0f;
  middleW = frameW - 64.0f;
  midX = left + 80.0f;

  sprite = hud->sprites[0x7e];
  sprite->posX = capX;
  GfxSpriteSetScaleRotation(sprite, 1.0f, scale, 0.0f, false);

  sprite = hud->sprites[0x7f];
  GfxSpriteSetSize(sprite, middleW, GfxSpriteGetHeight(sprite) * scale);
  sprite->posX = midX;

  sprite = hud->sprites[0x80];
  sprite->posX = (midX + frameW) - 64.0f;
  GfxSpriteSetScaleRotation(sprite, 1.0f, scale, 0.0f, false);

  if (UiTalkUsesNarrowIndent(hud) != 0) {
    hud->sprites[0x81]->posX = (float)((s32)hud->sprites[0x80]->posX - 0x50);
    hud->sprites[0x82]->posX = hud->sprites[0x81]->posX + 26.0f;
    hud->sprites[0x83]->posX = hud->sprites[0x81]->posX + 26.0f;
    hud->sprites[0x81]->posX = hud->sprites[0x81]->posX + 32.0f;
    for (i = 0x82; i < 0x84; i++) {
      GfxSpriteFlipU(hud->sprites[i]);
    }
    kind = hud->talkKind;
  }
  else {
    hud->sprites[0x81]->posX = (float)((s32)hud->sprites[0x7e]->posX - 0x30);
    hud->sprites[0x82]->posX = hud->sprites[0x81]->posX + 32.0f;
    hud->sprites[0x83]->posX = hud->sprites[0x81]->posX + 32.0f;
    kind = hud->talkKind;
  }
  if (kind == 0xf) {
    hud->sprites[0x81]->posX = hud->sprites[0x81]->posX + 6.0f;
  }

  /* Frame Y: 24 - (scale - 0.5) * 32, the offset clamped to -1..1.5 (NaN takes 1.5). */
  t = scale - 0.5f;
  for (i = 0x7e; i < 0x81; i++) {
    float y;

    if (t < -1.0f) {
      y = 24.0f - -1.0f * 32.0f;
    }
    else if (t <= 1.5f) {
      y = 24.0f - t * 32.0f;
    }
    else {
      y = 24.0f - 1.5f * 32.0f;
    }
    hud->sprites[i]->posY = y;
    hud->sprites[i]->posY = (float)(s32)hud->sprites[i]->posY;
  }
}
