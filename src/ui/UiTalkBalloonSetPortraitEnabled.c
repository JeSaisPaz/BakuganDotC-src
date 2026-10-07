// bdc 0x088cc694 UiTalkBalloonSetPortraitEnabled
#include "bdc.h"

/* Sets the global talk balloon portrait flag `g_uiTalkBalloonPortraitEnabled` (read by `UiTalkBalloonOpen` and
   `UiTalkBalloonUpdate`). Callers are field event commands. */

void UiTalkBalloonSetPortraitEnabled(u8 enabled)

{
  g_uiTalkBalloonPortraitEnabled = enabled;
  return;
}

