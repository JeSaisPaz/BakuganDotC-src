// bdc 0x089a45d8 UiMainMenuUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiMainMenu screen (task id 300): runs the current phase
   handler from the 7-entry pointer-to-member phase table `0x08a9f388` (indexed by `phase`, +0x28),
   then `UiMainMenuUpdateModels`, `UiMainMenuUpdateItemBoxLights`, then `UiScreenUpdateCommon` and, unless a close was
   requested, `UiScreenUpdateBg`. */

typedef struct MainMenuPhaseEntry {
  s16 thisAdjust; /* +0 */
  s16 vtIndex;    /* +2: nonzero = virtual, index into the vtable */
  void *fn;       /* +4: function, or vtable offset when virtual */
} MainMenuPhaseEntry;

typedef struct MainMenuVtblEntry {
  s16 thisAdjust;
  s16 pad;
  void (*fn)(void *);
} MainMenuVtblEntry;

void UiMainMenuUpdate(UiMainMenu *self)

{
  u32 phase = self->base.phase;
  u8 closing;

  if ((s32)phase >= 0 && phase < 7) {
    const MainMenuPhaseEntry *e = (const MainMenuPhaseEntry *)g_uiMainMenuPhaseTable + phase;
    u8 *obj = (u8 *)self + e->thisAdjust;
    void (*fn)(void *) = (void (*)(void *))e->fn;
    if (e->vtIndex != 0) {
      const MainMenuVtblEntry *v = (const MainMenuVtblEntry *)(*(u8 **)(obj + (intptr_t)e->fn)) + e->vtIndex;
      fn = v->fn;
      obj += v->thisAdjust;
    }
    fn(obj);
  }
  UiMainMenuUpdateModels(self);
  UiMainMenuUpdateItemBoxLights(self);
  closing = self->base.closeRequested;
  UiScreenUpdateCommon(&self->base);
  if (closing == 0) {
    UiScreenUpdateBg(&self->base);
  }
}
