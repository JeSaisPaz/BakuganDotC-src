// bdc 0x089efab0 CollisionDebugPrimCtor
#include "bdc.h"

/* Constructor of a debug primitive: `CoreObjectInit` with no chain, the
   `CollisionDebugPrim` vtable, appended to the debug chain (`CollisionDebugPrimLink`),
   kind and hasMtx cleared, lifetime 1 frame. Display list, colour and points are left
   untouched. Returns self. */

CollisionDebugPrim *CollisionDebugPrimCtor(CollisionDebugPrim *self)
{
  CoreObjectInit(&self->base, NULL);
  self->base.vtable = g_collisionDebugPrimVtbl;
  CollisionDebugPrimLink(self);
  self->kind = 0;
  self->hasMtx = 0;
  self->frames = 1;
  return self;
}
