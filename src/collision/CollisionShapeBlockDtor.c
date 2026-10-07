// bdc 0x089e51dc CollisionShapeBlockDtor
#include "bdc.h"

/* Destructor of a shape block: frees the mesh part array `parts`, the shape descriptor `desc` (only
   when `installed` is set) and the transformed-vertex buffer `worldVerts`, clearing each freed
   pointer, then frees the block itself when `flags & 1`. Does nothing for a NULL block. */

void CollisionShapeBlockDtor(void *block, u32 flags)
{
  CollisionShapeBlock *b = (CollisionShapeBlock *)block;
  void *p;

  if (b == NULL) {
    return;
  }
  p = b->parts;
  if (p != NULL) {
    MemLock();
    MemFree(p, NULL, 0);
    MemUnlock();
    b->parts = NULL;
  }
  if (b->installed != 0) {
    p = b->desc;
    if (p != NULL) {
      MemLock();
      MemFree(p, NULL, 0);
      MemUnlock();
      b->desc = NULL;
    }
  }
  p = b->worldVerts;
  if (p != NULL) {
    MemLock();
    MemFree(p, NULL, 0);
    MemUnlock();
    b->worldVerts = NULL;
  }
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(b, NULL, 0);
    MemUnlock();
  }
}
