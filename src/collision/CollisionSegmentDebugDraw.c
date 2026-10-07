// bdc 0x089e8f6c CollisionSegmentDebugDraw
#include "bdc.h"

/* Debug-draw method of the segment shape (type-2 vtable slot 6): adds a line debug primitive
   (0xa0-byte `CoreObject`, vtable `0x08af581c`, chained at `0x08ac5da8`; kind `+0x8c`, colour
   `+0x84`, lifetime in frames `+0x90`, matrix `+0x20`) from origin `+0x10` along `+0x20`
   (`CollisionDebugAddLine`). */

void CollisionSegmentDebugDraw(SegmentShape *seg, const ScePspFVector4 *colour, const ScePspFMatrix4 *mtx)

{
  CollisionDebugAddLine((ScePspFVector4 *)seg->start,seg->dir,colour,mtx);
  return;
}

