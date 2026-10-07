// bdc 0x088cc6a4 UiTalkBalloonSetTextColor
#include "bdc.h"

/* Sets the global talk balloon text colour index `g_uiTalkBalloonTextColor` (−1 = default white) and its
   16-bit parameter `g_uiTalkBalloonTextColorParam`, used by `UiTalkBalloonOpen`. */

void UiTalkBalloonSetTextColor(u8 color, u16 param)

{
  g_uiTalkBalloonTextColor = color;
  g_uiTalkBalloonTextColorParam = param;
  return;
}

