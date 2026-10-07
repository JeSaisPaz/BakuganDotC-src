// bdc 0x0891c390 UiHologramGalleryUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the hologram gallery screen (task id 391): runs the phase
   handler from the 5-entry table `g_uiHologramGalleryPhaseTable` and `UiScreenUpdateCommon`;
   `UiScreenUpdateBg` unless the close request was already set. */

void UiHologramGalleryUpdate(UiHologramGallery *self)

{
  u8 closeRequested;
  s32 phase = self->base.phase;

  if (phase >= 0 && phase < 5) {
    const MemberFnPtr *member = &g_uiHologramGalleryPhaseTable[phase];
    u8 *obj = (u8 *)self + member->delta;
    void *fn = member->pfn;

    if (member->index != 0) {
      const VtblEntry *entry =
          &(*(const VtblEntry **)(obj + (uintptr_t)member->pfn))[member->index];

      fn = entry->fn;
      obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
  }
  closeRequested = self->base.closeRequested;
  UiScreenUpdateCommon(&self->base);
  if (closeRequested == 0) {
    UiScreenUpdateBg(&self->base);
  }
  return;
}
