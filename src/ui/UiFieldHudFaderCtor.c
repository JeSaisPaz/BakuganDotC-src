// bdc 0x088c8d4c UiFieldHudFaderCtor
#include "bdc.h"

/* Constructor of the field HUD fader (`UiFieldHud+0x6c`, pointer to a 0xc-byte `{s32 state,
   sprites, f32 alpha}`): allocates the state from the low heap. Called by `UiFieldHudCtor`. */

void UiFieldHudFaderCtor(void **fader)

{
  bool fromLow;
  void *mem;
  void *result;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(UiFieldHudFader),(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  result = (void *)0x0;
  if (mem != (void *)0x0) {
    result = mem;
  }
  *fader = result;
  return;
}
