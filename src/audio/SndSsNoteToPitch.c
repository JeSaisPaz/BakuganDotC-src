// bdc 0x08a21274 SndSsNoteToPitch
#include "bdc.h"

/* Converts a MIDI note (+ fine tune in 1/128 semitones) relative to a tone's centre note into a SAS
   pitch factor (`0x1000` = original rate): semitone and fine steps index the tables
   `g_sndSsSemitonePitchTable` (12 semitone ratios) and `g_sndSsFinePitchTable` (128 fine
   ratios), the octave shifts the product (rounded right shift for octaves below the base). */

u16 SndSsNoteToPitch(s16 centerNote, s16 centerFine, s16 note, s16 fine)
{
  s32 sum;
  s32 fineOct;
  s32 fineIdx;
  s16 semis;
  s16 semi;
  s16 octave;
  s32 shift;
  u32 pitch;

  sum = fine + centerFine;
  fineOct = sum / 128;
  semis = (s16)(fineOct + (u16)note - (u16)centerNote);
  semi = semis % 12;
  octave = (s16)(semis / 12 - 2);
  fineIdx = sum - fineOct * 128;
  if (semi < 0 || (semi == 0 && fineIdx < 0)) {
    semi = (s16)(semi + 12);
    octave = (s16)(octave - 1);
  }
  shift = -octave;
  if (fineIdx < 0) {
    semi = (s16)(semi + fineOct - 1);
    fineIdx = sum + 0x80;
  }
  pitch = (u32)((s32)((u32)g_sndSsSemitonePitchTable[semi] * g_sndSsFinePitchTable[fineIdx]) >> 16);
  if (octave < 0) {
    pitch = (pitch + (1 << (shift - 1))) >> shift;
  }
  return (u16)pitch;
}
