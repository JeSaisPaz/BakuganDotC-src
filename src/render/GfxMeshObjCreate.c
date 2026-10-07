// bdc 0x08825710 GfxMeshObjCreate
#include "bdc.h"

/* Allocates a 0x1c0-byte mesh object (`GfxMeshObjCtor`) from the low heap (`GfxMeshObjCtor`) and
   initialises it: state `+0x18 = state`, `+0x1c = 0`, owner `+0x198 = owner`, `+0x20 = 4`, identity
   world matrix `+0x30`, texture scale `+0x144`/`+0x148 = 1.0`, `+0x178`/`+0x17c = 0`, primitive
   `+0x160 = 1`, `+0x164 = 8`. Returns the object; the `GfxMeshObjCreateList*` wrappers append it to
   one of the six draw lists. */

void * GfxMeshObjCreate(s32 state, void *owner)

{
  bool fromLow;
  GfxMeshObj *self;
  GfxMeshObj *obj;
  s32 i;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  self = MemAlloc(sizeof(GfxMeshObj), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  obj = NULL;
  if (self != NULL) {
    GfxMeshObjCtor(self);
    obj = self;
  }
  obj->state = state;
  obj->step = 0;
  obj->owner = owner;
  obj->mode = 4;
  /* Identity world matrix (vmidt.q M000 stored with four sv.q). */
  for (i = 0; i < 16; i++) {
    obj->basis[i] = (i % 5 == 0) ? 1.0f : 0.0f;
  }
  obj->scaleA = 1.0f;
  obj->scaleB = 1.0f;
  obj->texOffsetU = 0.0f;
  obj->texOffsetV = 0.0f;
  obj->patchDivS = 1;
  obj->patchDivT = 8;
  return obj;
}
