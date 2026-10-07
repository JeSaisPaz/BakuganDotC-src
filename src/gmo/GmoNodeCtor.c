// bdc 0x08a13ff4 GmoNodeCtor
#include "bdc.h"

/* In-place constructor of the 0xc0-byte model node records carved by `GmoPlanTakeNodes`:
   reference count 1, parent/indices -1, clears the links, and sets an identity local transform:
   translation `+0x50` = 0 (`GmoVec3Set`), rotation quaternion `+0x60` = (0,0,0,`0x08aa5290`)
   (`GmoVec4SetB`), scale `+0x70` = 1 (`GmoVec3Set`) and the matrix `+0x80`
   (`GmoMatrixScaleTranslate`). Returns `rec` (NULL-safe). */

GmoNode *GmoNodeCtor(GmoNode *self)

{
  float x;
  
  if (self != (GmoNode *)0x0) {
    self->parentIndex = -1;
    self->morphPos = 0.0f;
    self->color = -1;
    self->refCount = 1;
    self->tag = 0;
    self->parent = (GmoNode *)0x0;
    self->parts = (void **)0x0;
    self->morphWeights = (float *)0x0;
    self->block10 = (void *)0x0;
    self->block14 = (void *)0x0;
    self->partCount = 0;
    self->morphCount = 0;
    self->boneCount = 0;
    self->drawGroup = 0;
    self->flags = 0;
    self->visible = 0;
    self->matrix = (float *)0x0;
    self->block34 = (void *)0x0;
    self->block38 = (void *)0x0;
    self->flags42 = 0;
    GmoVec3Set(0.0f,0.0f,0.0f,self->translate);
    x = g_gmoUnitFloat;
    GmoVec4SetB(0.0f,0.0f,0.0f,g_gmoUnitFloat,self->rotate);
    GmoVec3Set(x,x,x,self->scale);
    GmoMatrixScaleTranslate(0.0f,0.0f,0.0f,x,x,x,self->localMatrix);
  }
  return self;
}

