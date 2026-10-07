// bdc 0x08a2088c SndSsFindVoicesByHandle
#include "bdc.h"

/* Returns the mask of hardware voices of the Sony sound layer that carry `handle`
   (`SndSsVoiceMaskByChannelAll(group, handle)` under the layer mutex); 0 when the layer is not
   initialised or `group` is above 15. `SndManagerProcessCommands` uses it with `group = 0` to
   find the voice of a control command whose handle is not in the manager's voice table. */

u32 SndSsFindVoicesByHandle(u32 group, s32 handle)

{
  u32 mask;
  
  if ((g_sndSsState == -1) || (0xf < group)) {
    mask = 0;
  }
  else {
    sceKernelLockLwMutex(&g_sndSsMutex,1,NULL);
    mask = SndSsVoiceMaskByChannelAll(group,handle);
    sceKernelUnlockLwMutex(&g_sndSsMutex,1);
  }
  return mask;
}

