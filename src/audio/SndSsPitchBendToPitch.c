// bdc 0x08a1fa58 SndSsPitchBendToPitch
#include "bdc.h"

/* Converts a 14-bit MIDI pitch-bend value (`0x2000` = centre) into a pitch delta for a voice of the
   Sony sound layer, under the layer mutex: reads the voice's tone parameters
   (`SndSsVoiceGetToneInfo`: bend ranges up/down, base note, fine tune, sample rate), converts
   note + bend to pitch with `SndSsNoteToPitch` and returns the difference scaled by `rate /
   44100` (negative for bends below centre). Values `> 0x2000` use the up range, `< 0x2000` the
   down range; the centre value or a zero bend range returns 0. Used by `SndSsSetVoicePitchBend`
   and the sequencer (`SndSsSeqNoteOn`, `SndSsSeqPitchBend`). */

s32 SndSsPitchBendToPitch(s32 voiceId, u32 bend14)
{
  u32 bendUp;
  u32 bendDown;
  u32 note;
  s32 centerNote;
  u32 centerNoteU;
  s32 fine;
  u32 sampleRate;
  s32 total;
  s32 semis;
  u16 basePitch;
  u16 bentPitch;
  s32 result;

  sceKernelLockLwMutex(&g_sndSsMutex, 1, NULL);
  SndSsVoiceGetToneInfo(voiceId, &bendUp, &bendDown, &note, &centerNote, &centerNoteU, &fine,
                        &sampleRate);
  if (bend14 > 0x2000) {
    result = 0;
    if (bendUp != 0) {
      total = (s32)(((bend14 - 0x2000) * bendUp * 0x80) / 0x1fff) + (centerNote * 0x7f) / 100;
      basePitch = SndSsNoteToPitch((s16)centerNoteU, (s16)((fine * 0x7f) / 100), (s16)note,
                                   (s16)((centerNote * 0x7f) / 100));
      bentPitch = SndSsNoteToPitch((s16)centerNoteU, (s16)((fine * 0x7f) / 100), (s16)note,
                                   (s16)total);
      result = (s32)((u32)((s32)(bentPitch - basePitch) * sampleRate) / 44100u);
    }
  }
  else {
    result = 0;
    if (bend14 < 0x2000 && bendDown != 0) {
      total = (s32)(((0x2000 - bend14) * bendDown * 0x80) / 0x2000) + (centerNote * 0x7f) / 100;
      basePitch = SndSsNoteToPitch((s16)centerNoteU, (s16)((fine * 0x7f) / 100), (s16)note,
                                   (s16)((centerNote * 0x7f) / 100));
      semis = total / 0x80;
      bentPitch = SndSsNoteToPitch((s16)centerNoteU, (s16)((fine * 0x7f) / 100),
                                   (s16)(u16)(note - semis), (s16)(semis * 0x80 - total));
      result = -(s32)((u32)((s32)(basePitch - bentPitch) * sampleRate) / 44100u);
    }
  }
  sceKernelUnlockLwMutex(&g_sndSsMutex, 1);
  return result;
}
