// bdc 0x08a2060c SndSsStopAll
#include "bdc.h"

/* Stops and resets all 32 voices of the Sony sound layer under its mutex: for each voice it runs
   the internal release (`SndSsVoiceSetSustain`) and stop (`SndSsVoiceKeyOff`) with the voice's
   bit of the active mask (`SndSasGetPauseFlag`). Returns 0, or the last real error code
   (`0x80450012` is ignored); `0x80450001` when the layer is not initialised.
   `SndManagerProcessCommands` calls it when the manager's reset word `+0x1a0` is negative. */

u32 SndSsStopAll(void)

{
  u32 mask;
  uint rc;
  uint ret;
  u32 voice;
  
  ret = 0x80450001;
  if (g_sndSsState != -1) {
    sceKernelLockLwMutex(&g_sndSsMutex,1,(u32 *)0x0);
    ret = 0;
    mask = SndSasGetPauseFlag();
    voice = 0;
    do {
      rc = SndSsVoiceSetSustain(voice,mask & 1,0);
      if ((rc >> 0x1f & (uint)(rc != 0x80450012)) != 0) {
        ret = rc;
      }
      rc = SndSsVoiceKeyOff(voice,mask & 1);
      voice = voice + 1;
      if ((rc >> 0x1f & (uint)(rc != 0x80450012)) != 0) {
        ret = rc;
      }
      mask = mask >> 1;
    } while (voice < 0x20);
    sceKernelUnlockLwMutex(&g_sndSsMutex,1);
  }
  return ret;
}

