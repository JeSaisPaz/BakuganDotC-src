// bdc 0x08a21730 SndSsSeqTick
#include "bdc.h"

/* Advances every active sequence of the Sony sound layer's sequencer by one output block: the block
   length in µs is `blockSize * 1000000 / 44100` (`g_sndSsSeqBlockSize`); for each of the 0x80
   track slots (`g_sndSsSeqTracks`) whose sequence is playing (`state == 1`) it reads delta times
   (`SndSsSeqReadDeltaTime`, converted with the tempo fields `tempo + tempoOffset` (clamped at 0)
   / `division` into `elapsed`) and processes events (`SndSsSeqProcessEvent`) until the
   accumulated time reaches the block, then subtracts the block length. A track whose state drops
   to 0 is left; an event result of -1 aborts the whole tick. Always returns 0. */

s32 SndSsSeqTick(void)

{
  u32 blockUs;
  u32 i;
  SndSsSeqTrack *track;
  s32 tempo;
  s32 ret;

  blockUs = (u32)(g_sndSsSeqBlockSize * 1000000) / 44100u;
  for (i = 0; i < 0x80; i++) {
    if (g_sndSsSeqTracks[i] == NULL || g_sndSsSeqTracks[i]->state != 1) {
      continue;
    }
    while (1) {
      track = g_sndSsSeqTracks[i];
      if (track->needDelta != 0) {
        SndSsSeqReadDeltaTime(track);
        track = g_sndSsSeqTracks[i];
        tempo = track->tempo + track->tempoOffset;
        if (tempo < 0) {
          tempo = 0;
        }
        track->elapsed += track->delta * (u32)(tempo / (s32)track->division);
      }
      track = g_sndSsSeqTracks[i];
      if (!(track->elapsed < blockUs)) {
        track->elapsed -= blockUs;
        track->needDelta = 0;
        break;
      }
      ret = SndSsSeqProcessEvent(track);
      track = g_sndSsSeqTracks[i];
      track->needDelta = 1;
      if (track->state == 0) {
        break;
      }
      if (ret == -1) {
        return 0;
      }
    }
  }
  return 0;
}
