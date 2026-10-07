// bdc 0x08931e8c UiGauntletSetupUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the gauntlet card setup screen (task id 373): runs the phase
   handler from the 4-entry PMF table `0x08a9c698` (`g_uiGauntletSetupPhaseTable`), then the model
   update `UiGauntletSetupUpdateModels`, `UiScreenUpdateCommon`, and unless the screen was
   closing (`closeRequested` sampled before the common update) `UiScreenUpdateBg`. */

typedef struct GauntletPhaseEntry {
  s16 thisAdjust; /* +0 */
  s16 vtIndex;    /* +2: nonzero = virtual, index into the vtable */
  void *fn;       /* +4: function, or vtable offset when virtual */
} GauntletPhaseEntry;

void UiGauntletSetupUpdate(UiGauntletSetup *self)

{
  u32 phase = self->base.phase;
  u8 closing;

  if ((s32)phase >= 0 && phase < 4) {
    const GauntletPhaseEntry *e = (const GauntletPhaseEntry *)g_uiGauntletSetupPhaseTable + phase;
    u8 *obj = (u8 *)self + e->thisAdjust;
    void (*fn)(void *) = (void (*)(void *))e->fn;
    if (e->vtIndex != 0) {
      const VtblEntry *v = (const VtblEntry *)(*(u8 **)(obj + (intptr_t)e->fn)) + e->vtIndex;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
  UiGauntletSetupUpdateModels(self);
  closing = self->base.closeRequested;
  UiScreenUpdateCommon(&self->base);
  if (closing == 0) {
    UiScreenUpdateBg(&self->base);
  }
}
