// bdc 0x089c7ef0 SndManagerUpdateVoices
#include "bdc.h"

/* Per-frame upkeep of the 32 `SndVoiceSlot`s, called at the end of `SndManagerProcessCommands`:
   for every slot with a handle it ramps the category volume of the voice, applies voice fade-outs
   (see `SndManagerStartVoiceFadeOut`), updates the hardware voice volume and frees the slot once
   the voice has ended. Returns 1 if at least one voice was found still sounding, else 0. */

u8 SndManagerUpdateVoices(SndManager *mgr)
{
  u32 voiceId = 0; /* last hardware voice seen; kept across slots */
  u8 anySounding = 0;
  s32 i;

  for (i = 0; i < 32; i++) {
    SndVoiceSlot *v = &mgr->voices[i];
    bool sounding;

    if (v->handle == 0)
      continue;

    sounding = false;
    if (v->voiceId != -1) {
      voiceId = v->voiceId;
      if (SndSsGetVoiceStatus(voiceId) == 0)
        sounding = true;
    }

    if (!sounding) {
      if ((v->flags2 & 1) == 0 && v->fadeTimer <= 0.0f)
        continue;
      v->handle = 0;
      v->fadeTimer = 0.0f;
      v->flags = v->flags & ~1;
      v->voiceId = -1;
      v->flags2 = v->flags2 & ~1;
      continue;
    }

    {
      u32 cat = ((u32)v->handle >> 28) & 7;
      float cur = mgr->catVolume[cat][0];
      float delta;
      float timer;
      float x;
      float factor;
      u32 vol;

      v->flags2 = v->flags2 | 1;
      delta = mgr->catVolume[cat][1] - cur;
      anySounding = 1;

      /* move the category volume towards its target by 1/fps per frame */
      if (delta != 0.0f) {
        if (delta < 0.0f) {
          cur = cur - 1.0f / (float)GfxDisplayGetFps(g_gfxDisplay);
          mgr->catVolume[cat][0] = cur;
          if (cur < mgr->catVolume[cat][1])
            mgr->catVolume[cat][0] = mgr->catVolume[cat][1];
        } else {
          cur = cur + 1.0f / (float)GfxDisplayGetFps(g_gfxDisplay);
          mgr->catVolume[cat][0] = cur;
          if (!(cur <= mgr->catVolume[cat][1]))
            mgr->catVolume[cat][0] = mgr->catVolume[cat][1];
        }
      }

      timer = v->fadeTimer;
      if (!(timer <= 0.0f)) {
        /* fade-out in progress */
        timer = timer - 1.0f / (float)GfxDisplayGetFps(g_gfxDisplay);
        v->fadeTimer = timer;
        if (timer <= 0.0f) {
          v->fadeTimer = 0.0f;
          SndSsStopHandle(v->handle);
          continue;
        }
        x = (1.0f - (0.125f - timer) * 8.0f) * 127.0f * v->gain;
        if (cat == 4)
          factor = SndManagerGetVoiceVolume(mgr);
        else
          factor = SndManagerGetMasterVolumeF(mgr);
        x = mgr->catVolume[cat][0] * factor * x;
        vol = (u32)(s32)x;
        if (vol >= 0x80)
          vol = 0x7f;
        SndSsSetVoiceVolume(voiceId, vol);
        continue;
      }

      if (delta == 0.0f)
        continue;

      x = v->gain * mgr->catVolume[cat][0];
      if (cat == 4)
        x = x * SndManagerGetVoiceVolume(mgr);
      else
        x = x * SndManagerGetMasterVolumeF(mgr);
      vol = (u32)(s32)(x * 127.0f);
      if (vol >= 0x80)
        vol = 0x7f;
      SndSsSetVoiceVolume(voiceId, vol);
      v->flags = 0;
    }
  }
  return anySounding;
}
