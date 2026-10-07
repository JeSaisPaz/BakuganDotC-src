// bdc 0x089f03c4 CollisionDebugPrimLink
#include "bdc.h"

/* Appends a debug primitive to the debug chain: becomes the head `0x08ac5da8` when the chain is
   empty, otherwise `CoreObjectAppend`. */

void CollisionDebugPrimLink(CollisionDebugPrim *self)

{
  if (g_collisionDebugPrims == (CollisionDebugPrim *)0x0) {
    g_collisionDebugPrims = self;
    return;
  }
  CoreObjectAppend(&self->base,&g_collisionDebugPrims->base);
  return;
}

