// bdc 0x08a20138 SndSsNoteOff
#include "bdc.h"

/* Sequencer note-off: under the layer mutex finds the voices playing `note` on MIDI channel
   `channel` for `handle` (`SndSsVoiceMaskByNote`) and keys each off with `SndSsVoiceKeyOff`
   (passing the voice's SAS pause bit). Returns the mask of voices whose key-off returned 0; 0 for
   a bad channel (≥ 16) / note (≥ 128) or when the layer is down. */

u32 SndSsNoteOff(u32 channel, u32 note, s32 handle)

{
  u8 chanNote[2];
  u32 pauseBits;
  u32 noteMask;
  u32 paused;
  u32 hit;
  u32 voice;
  u32 bit;
  u32 result;

  result = 0;
  if (g_sndSsState == -1 || !(channel < 0x10 && note < 0x80)) {
    return result;
  }
  chanNote[0] = (u8)channel;
  bit = 1;
  chanNote[1] = (u8)note;
  sceKernelLockLwMutex(&g_sndSsMutex, 1, NULL);
  result = 0;
  pauseBits = SndSasGetPauseFlag();
  noteMask = SndSsVoiceMaskByNote(chanNote, handle);
  voice = 0;
  if (g_sndSsMaxVoices != 0) {
    do {
      paused = pauseBits & 1;
      hit = noteMask & 1;
      pauseBits >>= 1;
      if (noteMask == 0) {
        break;
      }
      noteMask >>= 1;
      if (hit != 0) {
        if (SndSsVoiceKeyOff(voice, paused) == 0) {
          result |= bit;
        }
      }
      voice++;
      bit <<= 1;
    } while (voice < (u32)g_sndSsMaxVoices);
  }
  sceKernelUnlockLwMutex(&g_sndSsMutex, 1);
  return result;
}
