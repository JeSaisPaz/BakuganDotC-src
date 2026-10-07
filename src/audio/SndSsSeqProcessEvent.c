// bdc 0x08a219cc SndSsSeqProcessEvent
#include "bdc.h"

/* MIDI event interpreter of the Sony sound layer's sequencer: reads the next event at
   `track->cursor` with running status (a data byte first reuses `track->runningStatus`) and
   dispatches by status nibble: `0x90` note-on (`SndSsSeqNoteOn`), `0x80` note-off
   (`SndSsNoteOff` with the channel's `voiceHandle`), `0xa0` aftertouch and `0xd0` channel
   pressure (skipped), `0xb0` control change (`SndSsSeqControlChange`), `0xc0` program change
   (stored in the channel's `program`), `0xe0` pitch bend (`SndSsSeqPitchBend`), `0xff` meta events
   (`0x51` sets `tempo`, others skipped by length) and `0xf0`-`0xfe` sysex (skipped by its
   variable-length size). The status byte becomes the new running status, except at an end-of-track
   meta event (`ff 2f 00`): the track then rewinds to `smf + 0x16` with running status `0xff`, needs
   a fresh delta, clears `elapsed`/`delta`, counts down `repeatsLeft` unless `repeatMode == -1`, and
   stops (`state = 0`) once `repeatsLeft` is 0. Always returns 0. Called by `SndSsSeqTick`. */

s32 SndSsSeqProcessEvent(SndSsSeqTrack *track)
{
  const u8 *p;
  u8 status;
  u8 a;
  u8 b;
  u8 metaType = 0;
  u32 len = 0;
  u32 ch;

  p = track->cursor;
  status = *p;
  track->cursor = p + 1;
  if ((s8)status >= 0) {
    track->cursor = p;
    status = track->runningStatus;
  }
  ch = status & 0xf;

  switch (status & 0xf0) {
  case 0x80:
    p = track->cursor;
    a = p[0];
    track->cursor = p + 2;
    SndSsNoteOff(ch, a, track->channels[ch].voiceHandle);
    break;
  case 0x90:
    p = track->cursor;
    a = p[0];
    track->cursor = p + 1;
    b = p[1];
    track->cursor = p + 2;
    SndSsSeqNoteOn(status, a, b, track);
    break;
  case 0xa0:
    track->cursor = track->cursor + 2;
    break;
  case 0xb0:
    p = track->cursor;
    a = p[0];
    track->cursor = p + 1;
    b = p[1];
    track->cursor = p + 2;
    SndSsSeqControlChange(status, a, b, track);
    break;
  case 0xc0:
    p = track->cursor;
    a = p[0];
    track->cursor = p + 1;
    track->channels[ch].program = a;
    break;
  case 0xd0:
    track->cursor = track->cursor + 1;
    break;
  case 0xe0:
    p = track->cursor;
    a = p[0];
    track->cursor = p + 1;
    b = p[1];
    track->cursor = p + 2;
    SndSsSeqPitchBend(status, a, b, track);
    break;
  case 0xf0:
    if (status == 0xff) {
      p = track->cursor;
      metaType = p[0];
      track->cursor = p + 1;
      len = p[1];
      track->cursor = p + 2;
      if (metaType != 0x2f && metaType == 0x51) {
        track->tempo = (p[2] << 16) | (p[3] << 8) | p[4];
        track->cursor = p + 5;
      } else {
        track->cursor = track->cursor + len;
      }
    } else {
      u32 size;

      p = track->cursor;
      a = p[0];
      track->cursor = p + 1;
      size = a;
      if ((s8)a < 0) {
        size = a & 0x7f;
        do {
          p = track->cursor;
          b = p[0];
          track->cursor = p + 1;
          size = (size << 7) + (b & 0x7f);
        } while ((s8)b < 0);
      }
      track->cursor = track->cursor + size;
    }
    if (status == 0xff && metaType == 0x2f && len == 0) {
      if (track->repeatMode != -1)
        track->repeatsLeft = track->repeatsLeft - 1;
      track->runningStatus = 0xff;
      track->cursor = track->smf + 0x16;
      track->needDelta = 1;
      track->elapsed = 0;
      track->delta = 0;
      if (track->repeatsLeft == 0)
        track->state = 0;
      return 0;
    }
    break;
  }

  track->runningStatus = status;
  return 0;
}
