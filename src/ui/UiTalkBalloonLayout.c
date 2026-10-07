// bdc 0x088cbe28 UiTalkBalloonLayout
#include "bdc.h"

/* Lays out the talk balloon (`UiTalkBalloonCtor`) for `style` (two texture names `{frame, icon}`,
   either NULL, or `style` NULL): resets `textSize` to `g_uiTalkBalloonVecA` and the printer colour
   to `g_colorWhite`, clears `frameSprite`; with a style sets line height 19 (frame) or 22,
   computes the sizes when `textSize` is zero (`UiTalkBalloonComputeTextSize`,
   `UiTalkBalloonComputeFrameSize`), creates the frame sprite (kind 1, texture `"NonTexture"` when
   narrower than 256, `UiTalkBalloonIconCtor`) with `hasFrame` and the text origin, and the icon
   sprite (kind 2, `GfxFindTexture`) with `hasIcon` and its position. Without a style it clears
   `hasFrame` and `hasIcon`. */

void UiTalkBalloonLayout(UiTalkBalloon *self, void *unused, const void *style)

{
  char *const *names = (char *const *)style; /* [0] frame texture, [1] icon texture */
  UiTextPrinter *printer;
  GfxSprite *sprite;
  GfxSprite *mem;
  char *name;
  bool fromLow;
  float scale;

  self->textSize[0] = g_uiTalkBalloonVecA.x;
  self->textSize[1] = g_uiTalkBalloonVecA.y;
  self->textSize[2] = g_uiTalkBalloonVecA.z;
  self->textSize[3] = g_uiTalkBalloonVecA.w;
  printer = self->printer;
  printer->color[0] = g_colorWhite.x;
  printer->color[1] = g_colorWhite.y;
  printer->color[2] = g_colorWhite.z;
  printer->color[3] = g_colorWhite.w;
  self->frameSprite = (GfxSprite *)0x0;
  if (names == (char *const *)0x0) {
    self->hasFrame = 0;
    self->hasIcon = 0;
    return;
  }

  self->hasIcon = names[1] != (char *)0x0;
  self->lineHeight = (names[0] != (char *)0x0) ? 19.0f : 22.0f;
  if (self->textSize[0] == 0.0f && self->textSize[1] == 0.0f) {
    UiTalkBalloonComputeTextSize(self, self->frameSize);
    UiTalkBalloonComputeFrameSize(self, self->textSize);
  }
  /* 0x3affffd5: just under 1/512 */
  scale = self->textSize[0] * 0x1.ffffaap-10f * 1.02f;

  if (names[0] == (char *)0x0) {
    self->hasFrame = 0;
    self->origin[0] = 0.0f;
    self->origin[1] = 0.0f;
    self->origin[2] = 0.0f;
    self->origin[3] = 0.0f;
  }
  else {
    name = (self->textSize[0] < 256.0f) ? "NonTexture" : names[0];
    sprite = (GfxSprite *)0x0;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = (GfxSprite *)MemAlloc(400 /* PSP: UiTalkBalloonSprite (0x184) rounded up to 0x190 */, (const char *)0x0, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != (GfxSprite *)0x0) {
      UiTalkBalloonIconCtor(mem, GfxFindTextureOrNull(name), 1, self);
      sprite = mem;
    }
    self->frameSprite = sprite;
    self->hasFrame = 1;
    self->origin[2] = 0.0f;
    self->origin[3] = 0.0f;
    self->origin[0] = (float)(s32)((scale * 18.0f + 16.0f) - self->textSize[0] * 0.5f);
    self->origin[1] = (float)(s32)(17.0f - self->frameSize[1] * 0.5f);
  }

  if (names[1] == (char *)0x0) {
    self->hasIcon = 0;
    return;
  }
  sprite = (GfxSprite *)0x0;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = (GfxSprite *)MemAlloc(400 /* PSP: UiTalkBalloonSprite (0x184) rounded up to 0x190 */, (const char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != (GfxSprite *)0x0) {
    UiTalkBalloonIconCtor(mem, GfxFindTexture(names[1]), 2, self);
    sprite = mem;
  }
  self->iconSprite = sprite;
  self->hasIcon = 1;
  if (self->frameSprite != (GfxSprite *)0x0) {
    self->iconSprite->posX = (float)(s32)((self->textSize[0] * 0.5f - 14.0f) - scale * 28.0f);
    self->iconSprite->posY = (float)(s32)(self->frameSize[1] * 0.5f - 18.0f);
  }
  else {
    self->iconSprite->posX = self->textSize[0] - 30.0f;
    self->iconSprite->posY = self->textSize[1] - 22.0f;
  }
}
