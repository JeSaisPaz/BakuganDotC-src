// bdc 0x08a20dfc SndSsKeyOnToneVoice
#include "bdc.h"

/* Variant of `SndSsKeyOnTone` that keys a tone on a caller-chosen voice: rejects `voice` past the
   voice count (`0x80450011`), builds the key-on block (`SndSsBuildKeyOnParams`), cuts the voices
   of the tone's key group (`SndSsVoiceMaskByKeyGroup`, `SndSsVoiceSetSustain`,
   `SndSsVoiceKeyOff`) and queues the key-on on `voice` (`SndSsVoiceQueueKeyOn`) instead of
   allocating one. Returns `voice` when the queue call returns ≥ 0, else the build/queue error. */

s32 SndSsKeyOnToneVoice(u32 bankId, u32 voice, u32 tone, u32 channel, u32 note, s32 pitch, u32 flags, s32 handle, const void *volPan)

{
  SndSsKeyOnParams params;
  u32 pauseMask;
  u32 pauseBits;
  u32 groupMask;
  u32 bit;
  u32 v;
  s32 ret;

  if (voice >= (u32)g_sndSsMaxVoices) {
    return (s32)0x80450011;
  }
  ret = SndSsBuildKeyOnParams(bankId, tone, (u8)channel, note, pitch, flags, handle,
                              (const s32 *)volPan, (u32 *)&params);
  if (ret != 0) {
    return ret;
  }
  pauseMask = SndSasGetPauseFlag();
  if (params.keyGroup != 0) {
    v = 0;
    groupMask = SndSsVoiceMaskByKeyGroup(params.keyGroup);
    pauseBits = pauseMask;
    if (0 < (u32)g_sndSsMaxVoices) {
      do {
        if (groupMask == 0) {
          break;
        }
        bit = groupMask & 1;
        groupMask >>= 1;
        if (bit != 0) {
          SndSsVoiceSetSustain(v, pauseBits & 1, 0);
          SndSsVoiceKeyOff(v, pauseBits & 1);
        }
        v++;
        pauseBits >>= 1;
      } while (v < (u32)g_sndSsMaxVoices);
    }
  }
  /* the asm shifts the pause mask left (`sllv`), so only voice 0 can see its pause bit */
  ret = SndSsVoiceQueueKeyOn(voice, (pauseMask << (voice & 0x1f)) & 1, &params);
  if (ret >= 0) {
    ret = (s32)voice;
  }
  return ret;
}
