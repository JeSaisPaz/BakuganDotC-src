// bdc 0x089e51ac CollisionShapeBlockInit
#include "bdc.h"

/* Initialises the shape block of a collider (`block = collider + 0x30`): clears the mesh fields
   `parts`, `worldVerts`, `partCount`, `installed`, `reservedD8`, `matrix`, `desc` and `shape`, sets
   `dirty` to 1, and returns `block`. */

void *CollisionShapeBlockInit(void *block)

{
  CollisionShapeBlock *b = (CollisionShapeBlock *)block;

  b->parts = NULL;
  b->worldVerts = NULL;
  b->partCount = 0;
  b->installed = 0;
  b->reservedD8 = 0;
  b->dirty = 1;
  b->matrix = NULL;
  b->desc = NULL;
  b->shape = NULL;
  return block;
}
