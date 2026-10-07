// bdc 0x088cc6c4 UiTalkBalloonSetFrameStyle
#include "bdc.h"

/* Sets the global talk balloon frame style `g_uiTalkBalloonFrameStyle` (0 `win_npc`, 1 `win_npc01`,
   `UiTalkBalloonApplyFrameStyle`). */

void UiTalkBalloonSetFrameStyle(u8 style)

{
  g_uiTalkBalloonFrameStyle = style;
  return;
}

