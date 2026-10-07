// bdc 0x08a21e18 SndSsSeqNoteOn
#include "bdc.h"

/* Sequencer note-on for MIDI channel `status & 0xf` of `track`: velocity 0 is a note-off
   (`SndSsNoteOff`); otherwise, if the channel is enabled in `channelMask`, computes two volumes
   (track `volume` / `volume2` x channel volume x expression x velocity, /0x7f each) and two pans
   (track `pan` / `pan2` + channel pan - 0x40, clamped 0..0x7f), keys the note on
   (`SndSsProgramKeyOn`) with the channel program and, for every voice keyed on, records the
   velocity and applies the channel's current pitch bend if it is off-centre
   (`SndSsPitchBendToPitch`, `SndSsSetVoicePitchOffset`). Always returns 0. */

s32 SndSsSeqNoteOn(u32 status, u8 note, u8 velocity, SndSsSeqTrack *track)
{
  struct {
    u8 vol;
    u8 pan;
    u8 vol2;
    u8 pan2;
    s32 pitch;
  } volPan;
  SndSsSeqChannel *ch;
  u32 channel;
  u32 voices;
  u32 voice;
  u32 bend;
  s32 pan;
  s32 pan2;

  channel = status & 0xf;
  if (velocity == 0) {
    SndSsNoteOff(channel, note, track->channels[channel].voiceHandle);
  }
  else if (((track->channelMask >> channel) & 1) != 0) {
    ch = &track->channels[channel];
    pan = track->pan + ch->pan - 0x40;
    if (pan < 0) {
      pan = 0;
    }
    pan2 = track->pan2 + ch->pan - 0x40;
    if (pan2 < 0) {
      pan2 = 0;
    }
    if (pan2 > 0x7f) {
      pan2 = 0x7f;
    }
    volPan.pan2 = (u8)pan2;
    if (pan > 0x7f) {
      pan = 0x7f;
    }
    volPan.pan = (u8)pan;
    volPan.pitch = 0;
    volPan.vol = (u8)(((u32)((track->volume * ch->volume) / 0x7f * ch->expression) / 0x7f *
                       velocity) / 0x7f);
    volPan.vol2 = (u8)(((u32)((track->volume2 * ch->volume) / 0x7f * ch->expression) / 0x7f *
                        velocity) / 0x7f);
    voices = SndSsProgramKeyOn(track->bank, channel, ch->program, note, ch->sustain, &volPan,
                               ch->voiceHandle);
    if (voices != 0) {
      for (voice = 0; voice < 0x20; voice++) {
        if (((voices >> voice) & 1) != 0) {
          g_sndSsVoiceVelocity[voice] = velocity;
          bend = ch->pitchBend;
          if (bend != 0x2000) {
            SndSsSetVoicePitchOffset(voice, SndSsPitchBendToPitch(voice, bend));
          }
        }
      }
    }
  }
  return 0;
}
