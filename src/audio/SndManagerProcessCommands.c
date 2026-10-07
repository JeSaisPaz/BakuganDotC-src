// bdc 0x089c8230 SndManagerProcessCommands
#include "bdc.h"

/* Consumer side of the `SndManager` command queue, run once per frame by `SndManagerStateRun`
   (manager lock held): first `SndManagerUpdateGroupSlots` advances the bank loads; if the reset
   word at `+0x1a0` is negative it stops and resets every Sony-layer voice (`SndSsStopAll`),
   clears the word and discards the whole queue; otherwise it executes the 32 queued commands
   (`cmdId[i]`, `cmdHandle[i]`, `cmdParam[i]`, written by `SndManagerPlay`, `SndManagerStop`,
   `SndManagerSetVolume`, `SndManagerSetPan`) and finally calls `SndManagerUpdateVoices`. */

void SndManagerProcessCommands(SndManager *mgr)
{
  s32 voiceId = 0;  /* hardware voice of the last control command; persists across commands */
  s32 i;

  SndManagerUpdateGroupSlots(mgr);
  if (mgr->resetRequest < 0) {
    SndSsStopAll();
    mgr->resetRequest = 0;
    for (i = 0; i < 32; i++) {
      mgr->cmdHandle[i] = 0;
    }
  }
  else {
    for (i = 0; i < 32; i++) {
      s32 handle = mgr->cmdHandle[i];
      s32 cmd;
      s32 result;

      if (handle == 0) {
        continue;
      }
      cmd = mgr->cmdId[i];
      result = 0;
      if (cmd < 0 && cmd >= -10) {
        s32 voiceMask = -1;
        s32 j;

        for (j = 0; j < 32; j++) {
          if (mgr->voices[j].handle == handle && mgr->voices[j].voiceId >= 0) {
            voiceId = mgr->voices[j].voiceId;
            voiceMask = 1 << voiceId;
            break;
          }
        }
        if (voiceMask == -1) {
          u32 mask = SndSsFindVoicesByHandle(0, handle);
          if (mask == 0) {
            mgr->cmdHandle[i] = 0;
            continue;
          }
          for (voiceId = 0; voiceId < 32; voiceId++) {
            if (mask & 1) {
              break;
            }
            mask >>= 1;
          }
        }
        switch (cmd) {
        case -6:
          result = SndSsSetVoicePitchBend(voiceId, mgr->cmdParam[i]);
          break;
        case -5:
          result = SndSsSetVoicePan(voiceId, mgr->cmdParam[i]);
          break;
        case -4: {
          float level = (float)mgr->cmdParam[i];
          s32 cmdHandle = mgr->cmdHandle[i];
          u32 cat = ((u32)cmdHandle >> 28) & 7;
          float factor;

          for (j = 0; j < 32; j++) {
            if (mgr->voices[j].handle == cmdHandle) {
              s32 master;

              mgr->voices[j].flags |= 1;
              master = SndManagerGetMasterVolume(mgr);
              if (master > 0) {
                mgr->voices[j].gain = level / (float)master;
              }
              else {
                mgr->voices[j].gain = 0.0f;
              }
              break;
            }
          }
          if (cat == 4) {
            factor = SndManagerGetVoiceVolume(mgr);
          }
          else {
            factor = SndManagerGetMasterVolumeF(mgr);
          }
          level = mgr->catVolume[cat][0] * factor * level;
          result = SndSsSetVoiceVolume(voiceId, (s32)level);
          break;
        }
        case -3:
          result = SndSsSetVoicePitchOffset(voiceId, mgr->cmdParam[i]);
          break;
        case -2:
          break;
        case -1:
          result = SndSsStopVoice(voiceId);
          break;
        default:
          break;
        }
      }
      else {
        s32 slot;
        u32 soundIdx;

        soundIdx = (u32)cmd & 0xfffff;
        slot = -1;
        SndManagerResolveSoundWord(mgr, cmd, &slot, &soundIdx);
        if (slot >= 0) {
          s32 hwVoice = 0;
          s32 voice = SndManagerAllocVoice(mgr, cmd, mgr->cmdHandle[i]);

          if (voice >= 0) {
            hwVoice = SndSsKeyOn(mgr->groups[slot].bankId, soundIdx, 0x40, mgr->cmdHandle[i],
                                 mgr->voices[voice].keyOn);
            if (hwVoice >= 0) {
              mgr->voices[voice].voiceId = hwVoice;
              /* indexed by the hardware voice id, as in the asm */
              mgr->voices[hwVoice].flags2 &= ~1;
            }
          }
          if (hwVoice < 0) {
            mgr->voices[voice].handle = 0;
          }
        }
      }
      if (result == 0) {
        mgr->cmdHandle[i] = 0;
      }
    }
  }
  SndManagerUpdateVoices(mgr);
}
