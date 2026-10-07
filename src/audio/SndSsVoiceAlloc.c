// bdc 0x08a22854 SndSsVoiceAlloc
#include "bdc.h"

/* Allocates a voice for a key-on of priority `priority`: first any voice in use whose SAS end flag
   is set and that is not pending a change (reset with `SndSsVoiceReset`); otherwise steals a
   voice of priority ≤ `priority` (lowest envelope) from the free list, then the sustained list, then the
   active list
   (`SndSsVoiceListFindSteal`). Returns the voice index, `0x80450010` when nothing can be stolen,
   or `0x80450001` when the voice layer is down. */

s32 SndSsVoiceAlloc(u32 priority)
{
  u32 endFlag;
  u32 bit;
  s32 i;
  s32 found;

  if (g_sndSsVoiceState == -1)
    return (s32)0x80450001;

  endFlag = SndSasGetEndFlag();
  if ((s32)g_sndSsVoiceCount > 0) {
    u32 ready = endFlag & ~g_sndSsVoicePendingMask;
    bit = 1;
    for (i = 0; i < (s32)g_sndSsVoiceCount; i++) {
      if (g_sndSsVoices[i].state != 0 && (ready & bit) != 0) {
        SndSsVoiceReset(&g_sndSsVoices[i]);
        return i;
      }
      bit <<= 1;
    }
  }

  if (g_sndSsVoiceFreeCount != 0) {
    found = SndSsVoiceListFindSteal((u8 **)g_sndSsVoiceFreeList, g_sndSsVoiceFreeCount, priority);
    if (found != -1)
      return found;
  }
  if (g_sndSsVoiceSustainedCount != 0) {
    found = SndSsVoiceListFindSteal((u8 **)g_sndSsVoiceSustainedList, g_sndSsVoiceSustainedCount,
                                    priority);
    if (found != -1)
      return found;
  }
  if (g_sndSsVoiceActiveCount != 0) {
    found = SndSsVoiceListFindSteal((u8 **)g_sndSsVoiceActiveList, g_sndSsVoiceActiveCount,
                                    priority);
    if (found != -1)
      return found;
  }
  return (s32)0x80450010;
}
