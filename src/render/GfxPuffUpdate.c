// bdc 0x08828ab8 GfxPuffUpdate
#include "bdc.h"

/* Update of the sprite puff (`GfxPuffCtor`) (vtable `g_gfxPuffVtbl` slot 2): calls the kind
   handler for its kind byte `kind` through the pointer-to-member table `g_gfxPuffKindTable`
   (a nonzero `index` selects a virtual member through the vtable at `pfn`). */

void GfxPuffUpdate(GfxPuff *puff)

{
  const MemberFnPtr *member = &g_gfxPuffKindTable[puff->kind];
  u8 *self = (u8 *)puff + member->delta;
  void *fn = member->pfn;

  if (member->index != 0) {
    const VtblEntry *entry =
        &(*(const VtblEntry **)(self + (uintptr_t)member->pfn))[member->index];

    fn = entry->fn;
    self += entry->delta;
  }
  ((void (*)(void *))fn)(self);
  return;
}
