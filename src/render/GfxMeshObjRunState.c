// bdc 0x088255bc GfxMeshObjRunState
#include "bdc.h"

/* Calls the state handler of the mesh object (`GfxMeshObjCtor`) for its state `+0x18` through the
   member-pointer table `0x08ab9ebc` (`{s16 delta, s16 vindex, fn}` entries; virtual when `vindex`
   ≠ 0). */

void GfxMeshObjRunState(GfxMeshObj *self)
{
  const VtblEntry *entry = &g_gfxMeshObjStateTable[self->state];
  u8 *obj = (u8 *)self + entry->delta;
  void (*fn)(void *) = (void (*)(void *))entry->fn;

  if (entry->pad != 0) {
    const VtblEntry *v = (const VtblEntry *)*(void **)(obj + (intptr_t)fn) + entry->pad;
    fn = (void (*)(void *))v->fn;
    obj += v->delta;
  }
  fn(obj);
}
