// bdc 0x08808628 UiCopyrightTaskUpdate
#include "bdc.h"

/* Per-frame update of the copyright-notice task (vtable slot 2): calls the state method selected by
   `+0x10` through the GCC pointer-to-member table at `0x08a33a50` (2 entries:
   `UiCopyrightTaskStateShow`, `UiCopyrightTaskStateEnd`) and removes the task
   (`CoreTaskRemove` with destroy) once the byte at `+0x28` is set. */

void UiCopyrightTaskUpdate(UiCopyrightTask *task)
{
  s32 state = task->state;

  if (state >= 0 && state < 2) {
    const MemberFnPtr *e = &g_uiCopyrightTaskStateTable[state];
    u8 *obj = (u8 *)task + e->delta;
    void (*fn)(void *) = (void (*)(void *))e->pfn;
    if (e->index != 0) {
      const VtblEntry *v = *(const VtblEntry **)(obj + (intptr_t)e->pfn) + e->index;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
  if (task->removeMe != 0) {
    CoreTaskRemove(&task->base, true);
  }
}
