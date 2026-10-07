// bdc 0x08a26378 GmoGimCountChildren
#include "bdc.h"

/* Counts the child blocks of a GIM block whose type equals `type` (0 = all). */

s32 GmoGimCountChildren(const void *block, u32 type)

{
  const GmoGimBlock *parent = (const GmoGimBlock *)block;
  const GmoGimBlock *child;
  const u8 *end;
  s32 count = 0;

  if (parent == NULL) {
    return 0;
  }
  end = (const u8 *)parent + parent->size;
  for (child = (const GmoGimBlock *)((const u8 *)parent + parent->firstChild);
       (const u8 *)child < end;
       child = (const GmoGimBlock *)((const u8 *)child + child->size)) {
    if (type == 0 || type == child->type) {
      count++;
    }
  }
  return count;
}
