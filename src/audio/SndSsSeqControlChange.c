// bdc 0x08a22060 SndSsSeqControlChange
#include "bdc.h"

/* Sequencer MIDI control change on channel `status & 0xf` of `track`. 7 volume / 11 expression:
   stores the byte in the channel record, then for every voice carrying the channel's handle
   (`SndSsFindVoicesByHandle`) sets volume velocity*track->volume/127*chVolume/127*chExpression/127
   (`SndSsSetVoiceVolume`) and the same with track->volume2 (`SndSsSetVoiceVolume2`).
   10 pan: stores it and sets each voice's pan / pan2 to track pan (pan2) + channel pan - 0x40,
   clamped to 0..0x7f (`SndSsSetVoicePan`, `SndSsSetVoicePan2`). 64 sustain: on only for value
   0x7f, recorded and applied through `SndSsChannelSetSustain`. 6 (data entry): after a 99/0 mark,
   sets the loop count. 99 (NRPN): value 0 marks the loop point at the cursor; value 1 jumps back to
   it while loops remain (count 0 = forever), clearing it after the last one. Always returns 0. */

s32 SndSsSeqControlChange(u32 status, u8 controller, u8 value, SndSsSeqTrack *track)
{
  u32 ch = status & 0xf;
  SndSsSeqChannel *chan;
  u32 mask;
  u32 voice;

  if (controller == 10) {
    chan = &track->channels[ch];
    chan->pan = value;
    mask = SndSsFindVoicesByHandle(ch, chan->voiceHandle);
    if (mask == 0) {
      return 0;
    }
    for (voice = 0; voice < 32; voice++) {
      if ((mask >> voice) & 1) {
        s32 pan = track->pan + chan->pan - 0x40;
        s32 pan2 = track->pan2 + chan->pan - 0x40;
        if (pan < 0) pan = 0;
        if (pan2 < 0) pan2 = 0;
        if (pan > 0x7f) pan = 0x7f;
        if (pan2 > 0x7f) pan2 = 0x7f;
        SndSsSetVoicePan(voice, pan);
        SndSsSetVoicePan2(voice, pan2);
      }
    }
    return 0;
  }
  if (controller == 6) {
    if (track->loopMarkSet != 0) {
      track->loopCount = value;
      track->loopMarkSet = 0;
    }
    return 0;
  }
  if (controller == 7 || controller == 11) {
    chan = &track->channels[ch];
    if (controller == 7) {
      chan->volume = value;
    } else {
      chan->expression = value;
    }
    mask = SndSsFindVoicesByHandle(ch, chan->voiceHandle);
    if (mask == 0) {
      return 0;
    }
    for (voice = 0; voice < 32; voice++) {
      if ((mask >> voice) & 1) {
        s32 velocity = g_sndSsVoiceVelocity[voice];
        u8 vol = (u8)(velocity * track->volume / 127);
        u8 vol2 = (u8)(velocity * track->volume2 / 127);
        s32 chVolume = chan->volume;
        s32 chExpression = chan->expression;
        u8 out2;
        vol = (u8)(vol * chVolume / 127);
        vol2 = (u8)(vol2 * chVolume / 127);
        out2 = (u8)(vol2 * chExpression / 127);
        SndSsSetVoiceVolume(voice, (u8)(vol * chExpression / 127));
        SndSsSetVoiceVolume2(voice, out2);
      }
    }
    return 0;
  }
  if (controller == 64) {
    chan = &track->channels[ch];
    if (value == 0x7f) {
      chan->sustain = 1;
      SndSsChannelSetSustain(ch, 1, chan->voiceHandle);
    } else {
      chan->sustain = 0;
      SndSsChannelSetSustain(ch, 0, chan->voiceHandle);
    }
    return 0;
  }
  if (controller == 99) {
    if (value == 0) {
      track->loopMarkSet = 1;
      track->loopPoint = track->cursor;
    } else if (value == 1) {
      const u8 *loopPoint = track->loopPoint;
      s8 count = track->loopCount;
      if (loopPoint == NULL && count == -1) {
        return 0;
      }
      if (count == 0) {
        track->cursor = loopPoint;
      } else {
        track->loopCount = count - 1;
        if ((s8)(count - 1) == 0) {
          track->loopCount = -1;
          track->loopPoint = NULL;
        } else {
          track->cursor = loopPoint;
        }
      }
    }
  }
  return 0;
}
