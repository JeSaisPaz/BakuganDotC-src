// bdc 0x08a20cd8 SndSsKeyOnTone
#include "bdc.h"

/* Internal key-on of one bank tone: builds the 0x58-byte key-on block (`SndSsBuildKeyOnParams`),
   cuts every voice of the same key group (`keyGroup` → `SndSsVoiceMaskByKeyGroup`, each
   released with `SndSsVoiceSetSustain` and `SndSsVoiceKeyOff`, passing that voice's pause
   bit), allocates a voice for the tone's priority (`SndSsVoiceAlloc`) and queues the key-on
   (`SndSsVoiceQueueKeyOn`). Returns the voice index, `0x80450010` when no voice could be
   allocated, or the build/queue error. */

s32 SndSsKeyOnTone(u32 bankId, u32 tone, u32 channel, u32 note, s32 pitch, u32 flags, s32 handle, const void *volPan)

{
  SndSsKeyOnParams params;
  u32 pauseMask;
  u32 pauseBits;
  u32 groupMask;
  u32 bit;
  s32 voice;
  s32 ret;

  ret = SndSsBuildKeyOnParams(bankId, tone, (u8)channel, note, pitch, flags, handle,
                              (const s32 *)volPan, (u32 *)&params);
  if (ret != 0) {
    return ret;
  }
  pauseMask = SndSasGetPauseFlag();
  if (params.keyGroup != 0) {
    voice = 0;
    groupMask = SndSsVoiceMaskByKeyGroup(params.keyGroup);
    pauseBits = pauseMask;
    if (0 < g_sndSsMaxVoices) {
      do {
        if (groupMask == 0) {
          break;
        }
        bit = groupMask & 1;
        groupMask >>= 1;
        if (bit != 0) {
          SndSsVoiceSetSustain(voice, pauseBits & 1, 0);
          SndSsVoiceKeyOff(voice, pauseBits & 1);
        }
        voice++;
        pauseBits >>= 1;
      } while (voice < g_sndSsMaxVoices);
    }
  }
  voice = SndSsVoiceAlloc(params.priority);
  if (voice < 0) {
    return (s32)0x80450010;
  }
  /* the asm shifts the pause mask left (`sllv`), so only voice 0 can see its pause bit */
  ret = SndSsVoiceQueueKeyOn(voice, (pauseMask << voice) & 1, &params);
  if (ret == 0) {
    ret = voice;
  }
  return ret;
}
