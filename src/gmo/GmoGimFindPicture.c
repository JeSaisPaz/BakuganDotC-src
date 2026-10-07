// bdc 0x08a267cc GmoGimFindPicture
#include "bdc.h"

/* Returns picture block `index` (type 3) of a GIM file's root block, or NULL. */

void *GmoGimFindPicture(void *data, u32 size, s32 index)

{
  return GmoGimFindChild(GmoGimGetRootBlock(data), 3, index);
}
