// bdc 0x0882cca0 UiTalkShowBattleMessage
#include "bdc.h"

/* Shows talk message `msgId` with face `faceId` on the talk/HUD task (`UiGetTalkTask`) (id 0x6e,
   `BtlHudUpdate`) in style 2 for the duration from `UiTalkGetMessageDuration`:
   `UiTalkShowMessage(win, faceId, msgId, force, duration, 2, 0, 0, 0, unused)`; `unused` is
   passed through as the voice id. Returns what `UiTalkShowMessage` returns. Used by the battle
   advice code (`BtlHudAdviceTutorialIntro` and friends). */

char UiTalkShowBattleMessage(void *win, u32 faceId, s32 msgId, s32 unused, u8 force)
{
  s32 duration = UiTalkGetMessageDuration(win, msgId);
  return UiTalkShowMessage(win, faceId, msgId, force, duration, 2, 0, 0, 0, unused);
}
