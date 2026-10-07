// bdc 0x0895c7b4 UiEquipClosePhase
#include "bdc.h"

/* Phase 4 (phase table `0x08a9d5f8`) of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): waits until the shared background task 320 (`UiSharedBgCtor`) is gone
   (`CoreTaskExists`), then requests the screen's own close (`closeRequested`, +0x4c). */

void UiEquipClosePhase(UiEquip *self)

{
  s32 exists;
  
  exists = CoreTaskExists(0x140);
  if (exists == 0) {
    (self->base).closeRequested = '\x01';
  }
  return;
}

