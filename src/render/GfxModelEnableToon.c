// bdc 0x089e08f8 GfxModelEnableToon
#include "bdc.h"

/* Enables toon shading on a model through its virtual method `+0x4c` (`GfxModelSetToon` in the
   base model vtable `0x08af5484`) as `(model, 1, level)`; only caller `ActorSpawn`. */

void GfxModelEnableToon(GfxModel *self, s32 level)
{
    const GfxModelVtable *vt = (const GfxModelVtable *)self->base.vtable;
    vt->setToon((u8 *)self + vt->setToonAdjust, 1, level);
}
