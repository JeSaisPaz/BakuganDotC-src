// bdc 0x0881d750 GfxEffectDtor
#include "bdc.h"

/* Destructor of the effect object (vtable `g_gfxEffectVtbl` slot 1): restores the effect vtable,
   releases the attached sound emitter state when `+0x204` is set (`GfxMeshObjKill`), runs the
   base display-object destructor `GfxSpriteDtor(effect, 0)` and frees the object when `flags & 1`.
    */

void GfxEffectDtor(GfxEffect *effect, u32 flags)

{
  GfxMeshObj *self;
  
  if (effect != (GfxEffect *)0x0) {
    self = effect->meshObj;
    (effect->base).vtable = g_gfxEffectVtbl;
    if (self != (GfxMeshObj *)0x0) {
      GfxMeshObjKill(self);
    }
    GfxSpriteDtor((GfxSprite *)effect,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(effect,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

