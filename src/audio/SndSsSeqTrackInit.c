// bdc 0x08a21904 SndSsSeqTrackInit
#include "bdc.h"

/* Resets a sequencer track record of the Sony sound layer: state `+0` = 4, track volume `+3`/`+5` =
   0x7f, pan `+4`/`+6` = 0x40, running status `+7` and `+2`/`+8` = 0xff, `+9` = 1, tempo `+0x10` =
   1000000 us per quarter (120 BPM), channel-enable mask `+0x24` = all, timing fields cleared; then
   the 16 channel records at `+0x38` (stride 0x10): program 0, volume 0x7f, expression 0x7f, pan
   0x40, voice handle `+0x40` = `0xff000000 | trackId << 8 | channel`, pitch bend `+0x44` = 0x2000
   (centre). Returns 0. */

s32 SndSsSeqTrackInit(SndSsSeqTrack *track, u32 trackId)

{
  u32 i;

  track->needDelta = 1;
  track->volume2 = 0x7f;
  track->pan2 = 0x40;
  track->loopCount = -1;
  track->channelMask = 0xffffffff;
  track->smf = NULL;
  track->cursor = NULL;
  track->tempoOffset = 0;
  track->bank = -1;
  track->delta = 0;
  track->elapsed = 0;
  track->runningStatus = 0xff;
  track->repeatMode = 0;
  track->repeatsLeft = 0;
  track->volume = 0x7f;
  track->pan = 0x40;
  track->loopPoint = NULL;
  track->loopMarkSet = 0;
  track->state = 4;
  track->tempo = 1000000;
  for (i = 0; i < 0x10; i++) {
    SndSsSeqChannel *ch = &track->channels[i];
    ch->voiceHandle = (trackId << 8 | i) | 0xff000000;
    ch->program = 0;
    ch->volume = 0x7f;
    ch->expression = 0x7f;
    ch->pan = 0x40;
    ch->pitchBend = 0x2000;
    ch->sustain = 0;
  }
  return 0;
}
