// bdc 0x0882c450 UiTalkGetMessageDuration
#include "bdc.h"

/* Looks up the display time of talk message `msgId` in the 236-entry table `g_talkMessageDurations` of `{s16
   msgId, s16 frames}` pairs; returns 90 frames when it is not listed. The first argument is unused.
   Used by `UiTalkShowBattleMessage`. */

s32 UiTalkGetMessageDuration(void *win, s32 msgId)
{
  s16 *entry = g_talkMessageDurations;
  s32 i = 0;
  do {
    i++;
    if (msgId == entry[0]) {
      return entry[1];
    }
    entry += 2;
  } while (i < 0xec);
  return 0x5a;
}
