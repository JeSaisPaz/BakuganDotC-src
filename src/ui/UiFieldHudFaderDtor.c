// bdc 0x088c8dc8 UiFieldHudFaderDtor
#include "bdc.h"

/* Destructor of the field HUD fader (`UiFieldHud+0x6c`, pointer to a 0xc-byte `{s32 state, sprites,
   f32 alpha}`): frees the state and, when `flags & 1`, the holder. Called by `UiFieldHudDtor`. */

void UiFieldHudFaderDtor(void **fader, u32 flags)

{
  void *ptr;
  
  if (fader != (void **)0x0) {
    ptr = *fader;
    if (ptr != (void *)0x0) {
      MemLock();
      MemFree(ptr,(char *)0x0,0);
      MemUnlock();
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(fader,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

