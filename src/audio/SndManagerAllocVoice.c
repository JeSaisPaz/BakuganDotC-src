// bdc 0x089c69e4 SndManagerAllocVoice
#include "bdc.h"

/* Takes the first free voice slot of the `SndManager` (`handle == 0`) for a play command being
   executed and initialises it: `handle`, `flags = 0`, `soundWord`, `voiceId = -1` (no hardware
   voice yet), `fadeTimer = 0`, `gain = 1.0`, and the 8-byte key-on parameter block (`keyOn`)
   copied from the 8 bytes at `+4` (`masterVolume` onwards) of the audio settings block
   `g_soundAudioSettings` (`{volL, panL, volR, panR}`-like bytes, defaults `0x7f 0x40 0x7f 0x40`).
   For voices of category 4 (handle bits 28..30 = 4, the speech/voice category) it replaces both
   volume bytes by `voiceVolume * 127` clamped to `0..0x7f` (`SndManagerGetVoiceVolume`). If the
   handle has bit 27 set (the `flag` argument of `SndManagerPlay`, i.e. start muted) the gain and
   both volume bytes are cleared. Returns the voice index, or -1 when all 32 voices are in use. */

s32 SndManagerAllocVoice(SndManager *mgr, u32 soundWord, s32 handle)
{
  SndVoiceSlot *slot;
  s32 i;
  s32 vol;

  for (i = 0; i < 32; i++) {
    slot = &mgr->voices[i];
    if (slot->handle != 0) {
      continue;
    }
    slot->handle = handle;
    slot->flags = 0;
    slot->soundWord = soundWord;
    slot->gain = 1.0f;
    slot->fadeTimer = 0.0f;
    slot->voiceId = -1;
    /* two word copies (lw/lw/sw/sw) of settings bytes +4..+0xb */
    memcpy(slot->keyOn, &g_soundAudioSettings->masterVolume, sizeof(slot->keyOn));
    if (((handle >> 28) & 7) == 4) {
      vol = (s32)(SndManagerGetVoiceVolume(mgr) * 127.0f);
      if (vol < 0) {
        vol = 0;
      }
      if (vol > 0x7f) {
        vol = 0x7f;
      }
      slot->keyOn[2] = (u8)vol;
      slot->keyOn[0] = (u8)vol;
    }
    if (((handle >> 27) & 1) != 0) {
      slot->gain = 0.0f;
      slot->keyOn[2] = 0;
      slot->keyOn[0] = 0;
    }
    return i;
  }
  return -1;
}
