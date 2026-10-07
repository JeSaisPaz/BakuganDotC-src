// bdc 0x08a224e8 SndSsSeqPitchBend
#include "bdc.h"

/* Sequencer MIDI pitch-bend event for channel `status & 0xf`: stores the 14-bit value `msb << 7 |
   lsb` (clamped to 0x3fff) in the channel record (`channels[ch].pitchBend`) and applies it to every
   voice of that channel (`SndSsFindVoicesByHandle`) through `SndSsPitchBendToPitch` and
   `SndSsSetVoicePitchOffset`. Returns 0. */

s32 SndSsSeqPitchBend(u32 status, u32 lsb, u32 msb, SndSsSeqTrack *track)

{
  u32 ch = status & 0xf;
  u32 bend14 = ((msb & 0xff) << 7) | (lsb & 0xff);
  u32 mask;
  u32 voice;
  u32 bits;

  if (0x3fff < bend14) {
    bend14 = 0x3fff;
  }
  track->channels[ch].pitchBend = bend14;
  mask = SndSsFindVoicesByHandle(ch, track->channels[ch].voiceHandle);
  if (mask != 0) {
    voice = 0;
    bits = mask;
    do {
      if ((bits & 1) != 0) {
        s32 offset = SndSsPitchBendToPitch(voice, bend14);
        SndSsSetVoicePitchOffset(voice, offset);
      }
      voice = voice + 1;
      bits = mask >> (voice & 0x1f);
    } while (voice < 0x20);
  }
  return 0;
}
