// bdc 0x0892b784 UiBakuganSelectUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiBakuganSelect screen (task id 371): runs the current
   phase handler from the 4-entry pointer-to-member phase table `g_uiBakuganSelectPhaseTable` (indexed by
   `phase`, +0x28), then `UiBakuganSelectUpdateModel`, then `UiScreenUpdateCommon` and, unless a close
   was requested, `UiScreenUpdateBg`. */

void UiBakuganSelectUpdate(UiBakuganSelect *self)

{
  u32 phase = self->base.phase;
  if ((s32)phase >= 0 && phase < 4) {
    const MemberFnPtr *e = &g_uiBakuganSelectPhaseTable[phase];
    u8 *obj = (u8 *)self + e->delta;
    void (*fn)(void *) = (void (*)(void *))e->pfn;
    if (e->index != 0) {
      const VtblEntry *v = *(const VtblEntry **)(obj + (intptr_t)e->pfn) + e->index;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
  UiBakuganSelectUpdateModel(self);
  u8 closing = self->base.closeRequested;
  UiScreenUpdateCommon(&self->base);
  if (closing == 0) {
    UiScreenUpdateBg(&self->base);
  }
}
