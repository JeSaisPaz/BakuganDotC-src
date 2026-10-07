// bdc 0x08a207e4 SndSsKeyOn
#include "bdc.h"

/* Starts a sound in the Sony sound layer under the layer's mutex: forwards to the internal key-on
   `SndSsKeyOnTone(bankId, soundIdx, 0xff, velocity, 0, 0, handle, params)`, which finds a free
   hardware voice (stealing voices that carry the same handle), sets it up from the bank and returns
   its voice id (negative on failure). `SndManagerProcessCommands` calls it as
   `SndSsKeyOn(group.bankId, index, 0x40, handle, &voice.keyOn)` for every play command. */

s32 SndSsKeyOn(s32 bankId, u32 soundIdx, s32 velocity, s32 handle, void *params)

{
  s32 voice;
  
  sceKernelLockLwMutex(&g_sndSsMutex,1,NULL);
  voice = SndSsKeyOnTone(bankId,soundIdx,0xff,velocity,0,0,handle,params);
  sceKernelUnlockLwMutex(&g_sndSsMutex,1);
  return voice;
}

