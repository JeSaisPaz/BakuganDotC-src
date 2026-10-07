// bdc 0x089b40a0 UiComboListOpenPhase
#include "bdc.h"

/* Phase 1 of the combo list screen: creates 0x36 layout sprites (`UiLayoutCreateSprites`) and the
   list (`UiComboListResetPage`, `UiComboListCreateSprites`), then runs the zoom-in tween (`UiComboListBounceStep` on
   `+0x88/+0x78/+0x7c`) and advances when it ends. */

void UiComboListOpenPhase(UiComboList *self)

{
  GfxSprite **outSprites;
  
  int step = self->step;
  if (step < 1) {
    if (step < 0) {
      return;
    }
    MemLock();
    bool wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    outSprites = MemAlloc(101 * sizeof(GfxSprite *),(char *)0x0,0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    (self->base).data = outSprites;
    UiLayoutCreateSprites((self->base).spriteLayer,outSprites,0x36);
    UiComboListResetPage(self);
    UiComboListCreateSprites(self);
    self->step = self->step + 1;
  }
  else if (1 < step) {
    return;
  }
  bool done = UiComboListBounceStep(&self->bounceAmplitude,&self->animFrame,&self->zoom);
  if (done) {
    int phase = (self->base).phase;
    self->bounceAmplitude = 0.0;
    self->step = 0;
    (self->base).phase = phase + 1;
  }
  return;
}

