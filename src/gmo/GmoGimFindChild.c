// bdc 0x08a263e8 GmoGimFindChild
#include "bdc.h"

/* Returns the `index`-th (0-based) child block of a GIM block with type `type` (0 = any), or NULL.
    */

void *GmoGimFindChild(const void *block, u32 type, s32 index)

{
  const GmoGimBlock *parent = (const GmoGimBlock *)block;
  const GmoGimBlock *child;
  const u8 *end;

  if (parent == NULL) {
    return NULL;
  }
  end = (const u8 *)parent + parent->size;
  for (child = (const GmoGimBlock *)((const u8 *)parent + parent->firstChild);
       (const u8 *)child < end;
       child = (const GmoGimBlock *)((const u8 *)child + child->size)) {
    if (type == 0 || type == child->type) {
      index--;
      if (index == -1) {
        return (void *)child;
      }
    }
  }
  return NULL;
}
