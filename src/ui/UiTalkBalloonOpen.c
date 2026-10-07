// bdc 0x088cc510 UiTalkBalloonOpen
#include "bdc.h"

/* Opens a talk balloon for field event messages: stores (112, 200, 0, 0) in
   `g_uiTalkBalloonVecB` and creates the balloon there with the `g_uiTalkBalloonFont` font and
   `g_uiTalkBalloonStyle` style (`UiTalkBalloonCreate`), font mode 1, line height 16,
   `pageFlags[1]` set, reveal timer 1000 (whole page at once), sets the speaker
   (`UiTalkBalloonSetSpeaker`), the portrait (`UiTalkBalloonShowPortrait`, if
   `g_uiTalkBalloonPortraitEnabled`), the name window (`UiTalkBalloonApplyFrameStyle`, unless
   `g_uiTalkBalloonTextColor` is 0xff), white text and outline colour (`g_colorWhite`), and
   shows parts 5 (cell 0, 2) and 6. Returns the task. */

UiTalkBalloon *UiTalkBalloonOpen(void *owner, s16 speaker)

{
  UiTalkBalloon *self;
  UiTextPrinter *printer;
  float pos[4];

  pos[0] = 112.0f;
  pos[1] = 200.0f;
  pos[2] = 0.0f;
  pos[3] = 0.0f;
  g_uiTalkBalloonVecB.x = pos[0];
  g_uiTalkBalloonVecB.y = pos[1];
  g_uiTalkBalloonVecB.z = pos[2];
  g_uiTalkBalloonVecB.w = pos[3];
  self = UiTalkBalloonCreate(owner, pos, g_uiTalkBalloonFont, g_uiTalkBalloonStyle);
  printer = self->printer;
  UiTextPrinterSetFont(printer, 1);
  self->lineHeight = 16.0f;
  self->pageFlags[1] = 1;
  self->revealTimer = 1000.0f;
  UiTalkBalloonSetSpeaker(self, speaker);
  if (g_uiTalkBalloonPortraitEnabled != 0) {
    UiTalkBalloonShowPortrait(self);
  }
  if (g_uiTalkBalloonTextColor != 0xff) {
    UiTalkBalloonApplyFrameStyle(self);
  }
  printer->color[0] = g_colorWhite.x;
  printer->color[1] = g_colorWhite.y;
  printer->color[2] = g_colorWhite.z;
  printer->color[3] = g_colorWhite.w;
  printer->outlineColor[0] = g_colorWhite.x;
  printer->outlineColor[1] = g_colorWhite.y;
  printer->outlineColor[2] = g_colorWhite.z;
  printer->outlineColor[3] = g_colorWhite.w;
  self->parts[5]->flags |= 1;
  GfxSpriteSetCell(self->parts[5], 0.0f, 2.0f);
  self->parts[6]->flags |= 1;
  return self;
}
