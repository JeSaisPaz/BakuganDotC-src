// bdc 0x089efb00 CollisionDebugPrimCopyCtor
#include "bdc.h"

/* Copy constructor of a debug primitive: constructs self like `CollisionDebugPrimCtor`
   (`CoreObjectInit`, vtable, `CollisionDebugPrimLink`), copies the matrix, from/to
   points, colour, hasMtx and kind from src, and sets the lifetime to 1 frame (not copied).
   The display list is not copied. Returns self.
   The matrix and point copies are lv.q/sv.q quad copies; no VFPU value is live at return. */

CollisionDebugPrim *CollisionDebugPrimCopyCtor(CollisionDebugPrim *self, const CollisionDebugPrim *src)
{
  CoreObjectInit(&self->base, NULL);
  self->base.vtable = g_collisionDebugPrimVtbl;
  CollisionDebugPrimLink(self);
  self->mtx = src->mtx;
  self->from = src->from;
  self->to = src->to;
  self->colour = src->colour;
  self->hasMtx = src->hasMtx;
  self->kind = src->kind;
  self->frames = 1;
  return self;
}
