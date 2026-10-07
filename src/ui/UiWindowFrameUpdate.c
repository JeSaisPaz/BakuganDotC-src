// bdc 0x089fe730 UiWindowFrameUpdate
#include "bdc.h"

/* Update (vtable slot `+0x14`) of a 9-slice window frame (`UiWindowFrame`, 0x130 bytes: a
   `GfxSpriteLayer` with vtable `g_uiWindowFrameVtable` at `+0x74`, plus a `CoreObject`
   at `+0x80`): runs the animation-state handler for `+0xf4` from the member-pointer table
   `0x08ac6200` (0 = nothing, 1 = `UiWindowFrameStepOpen`, 2 = `UiWindowFrameStepClose`), then
   the layout virtual (`+0x3c`, `UiWindowFrameLayout`). */

void UiWindowFrameUpdate(UiWindowFrame *self)

{
  GfxSpriteLayer *layer = (GfxSpriteLayer *)self;
  const VtblEntry *layout;

  if (self->state >= 0 && (u32)self->state < 3) {
    const MemberFnPtr *member = &g_uiWindowFrameStateFns[self->state];
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
  layout = &layer->vtbl[7];
  ((void (*)(void *))layout->fn)((u8 *)self + layout->delta);
}
