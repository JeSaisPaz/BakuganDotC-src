// bdc 0x08804adc UiNameEntryUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the player name entry screen (task id 3000): runs the phase
   handler from the 5-entry pointer-to-member table `g_uiNameEntryPhaseTable` (indexed by
   `phase`); when the `done` byte is set it removes itself with `CoreTaskRemove``(task, true)`. */

typedef struct NameEntryVtblEntry {
  s16 thisAdjust;
  s16 pad;
  void (*fn)(void *);
} NameEntryVtblEntry;

void UiNameEntryUpdate(UiNameEntry *self)
{
  u32 phase = (u32)self->base.phase;

  if ((s32)phase >= 0 && phase < 5) {
    const MemberFnPtr *e = &g_uiNameEntryPhaseTable[phase];
    u8 *obj = (u8 *)self + e->delta;
    void (*fn)(void *) = (void (*)(void *))e->pfn;

    if (e->index != 0) {
      /* virtual: pfn holds the offset of the vtable pointer, index selects the slot */
      const NameEntryVtblEntry *v =
          (const NameEntryVtblEntry *)(*(u8 **)(obj + (intptr_t)e->pfn)) + e->index;
      fn = v->fn;
      obj += v->thisAdjust;
    }
    fn(obj);
  }
  if (self->done != 0) {
    CoreTaskRemove(&self->base.base, true);
  }
}
