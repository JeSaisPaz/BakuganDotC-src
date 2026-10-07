// bdc 0x08a20700 SndSsStoreVoiceVolumes
#include "bdc.h"

/* Records the four SAS volumes last applied to voice `voice` in `g_sndSsVoiceVolumes` (16 bytes
   per voice: left, right, effect left, effect right). */

void SndSsStoreVoiceVolumes(s32 voice, s32 left, s32 right, s32 effectLeft, s32 effectRight)

{
  g_sndSsVoiceVolumes[voice][3] = effectRight;
  g_sndSsVoiceVolumes[voice][0] = left;
  g_sndSsVoiceVolumes[voice][1] = right;
  g_sndSsVoiceVolumes[voice][2] = effectLeft;
}
