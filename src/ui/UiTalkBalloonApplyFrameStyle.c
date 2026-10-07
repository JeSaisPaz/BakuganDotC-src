// bdc 0x088cc48c UiTalkBalloonApplyFrameStyle
#include "bdc.h"

/* Shows the name window of the talk balloon (`UiTalkBalloonCtor`) with texture `"win_npc"` (style
   0) or `"win_npc01"` (style 1) according to `g_uiTalkBalloonFrameStyle` (`UiTalkBalloonSetFrameStyle`). */

void UiTalkBalloonApplyFrameStyle(UiTalkBalloon *self)

{
  GfxSprite *sprite;

  self->parts[4]->flags = self->parts[4]->flags | 1;
  if (g_uiTalkBalloonFrameStyle == '\0') {
    sprite = self->parts[4];
    sprite->texture = GfxFindTexture("win_npc");
  }
  else if (g_uiTalkBalloonFrameStyle < 2) {
    sprite = self->parts[4];
    sprite->texture = GfxFindTexture("win_npc01");
  }
  return;
}

