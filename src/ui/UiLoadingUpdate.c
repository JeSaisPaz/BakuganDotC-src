// bdc 0x0890a874 UiLoadingUpdate
#include "bdc.h"

/* Update (vtable slot `+0x14`) of the now-loading screen (task 10100 / 0x2774, 0x240 bytes, vtable
   `0x08af47dc`, `UiLoadingCtor`; shared UI objects `0x08ac0e80`): dispatches the state `+0x10`
   (0..4) through the member-pointer table `g_uiLoadingStateTable`: `UiLoadingStateInit`,
   `UiLoadingStateSetup`, `UiLoadingStateWaitDisc`, `UiLoadingStateIdle`,
   `UiLoadingStateClose`. */

void UiLoadingUpdate(UiLoading *self)

{
  s32 state = self->state;

  if (state >= 0 && state < 5) {
    const MemberFnPtr *member = &g_uiLoadingStateTable[state];
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
  return;
}
