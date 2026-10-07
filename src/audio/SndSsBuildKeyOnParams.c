// bdc 0x08a20f50 SndSsBuildKeyOnParams
#include "bdc.h"

/* Fills the 0x58-byte key-on block `out` (`SndSsKeyOnParams`) for a tone of bank `bankId`:
   tone/VAG data from `SndSsBankGetToneParams` (at `toneData`), channel and note, the pitch (note
   → `SndSsNoteToPitch` scaled by the sample rate / 44100, or the raw `pitch` when `note ==
   0xff`, plus `volPan[1]`, clamped 1..0x4000), the `volPan` words, `flags`, handle and bank id.
   Returns 0, `0x80450001` (layer down), `0x8045000a` (bad note/flags), `0x80450002` (unknown
   bank) or the error of `SndSsBankGetToneParams`. */

s32 SndSsBuildKeyOnParams(u32 bankId, u32 tone, u8 channel, u32 note, s32 pitch, u32 flags, s32 handle, const s32 *volPan, u32 *out)

{
  SndSsKeyOnParams *params = (SndSsKeyOnParams *)out;
  u16 notePitch;
  s32 ret;
  s32 value;

  if (g_sndSsState == -1) {
    return (s32)0x80450001;
  }
  if (note >= 0x80 && note != 0xff) {
    return (s32)0x8045000a;
  }
  if (flags >= 2) {
    return (s32)0x8045000a;
  }
  if (bankId >= 0x80 || g_sndSsBankTable[bankId] == 0) {
    return (s32)0x80450002;
  }
  ret = SndSsBankGetToneParams(bankId, tone, params->toneData);
  if (ret != 0) {
    return ret;
  }
  params->channel = channel;
  params->note = (u8)note;
  if ((note & 0xff) == 0xff) {
    value = pitch + volPan[1];
  }
  else {
    notePitch = SndSsNoteToPitch((s16)params->centerNote, (s16)((params->centerFine * 0x7f) / 100),
                                 (s16)(note & 0xff), (s16)((params->fineTune * 0x7f) / 100));
    value = (s32)((params->sampleRate * notePitch) / 44100u) + volPan[1];
  }
  if (value > 0x4000) {
    value = 0x4000;
  }
  if (value < 1) {
    value = 1;
  }
  params->pitch = value;
  params->volPan[0] = volPan[0];
  params->flags = flags;
  params->volPan[1] = volPan[1];
  params->handle = handle;
  params->bankId = bankId;
  return 0;
}
