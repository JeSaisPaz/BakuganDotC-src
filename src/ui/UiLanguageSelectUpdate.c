// bdc 0x08809e44 UiLanguageSelectUpdate
#include "bdc.h"

/* Update of the language-selection screen (id 199): calls the state method selected by `+0x10`
   (0..4) through the pointer-to-member table `g_uiLanguageSelectStateTable` and removes the task
   once the done byte `+0x58` is set. */

void UiLanguageSelectUpdate(CoreTask *task)

{
  UiLanguageSelect *self = (UiLanguageSelect *)task;
  s32 state = self->state;

  if (state >= 0 && state < 5) {
    const MemberFnPtr *e = &g_uiLanguageSelectStateTable[state];
    u8 *obj = (u8 *)self + e->delta;
    void (*fn)(void *) = (void (*)(void *))e->pfn;
    if (e->index != 0) {
      const VtblEntry *v = *(const VtblEntry **)(obj + (intptr_t)e->pfn) + e->index;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
  if (self->done != 0) {
    CoreTaskRemove(task, true);
  }
}
