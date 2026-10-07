// bdc 0x08a260a0 GmoGimGetRootBlock
#include "bdc.h"

/* Returns the GIM root block (`data + 0x10`, after the 16-byte signature), or NULL for NULL. */

void *GmoGimGetRootBlock(void *data)

{
  if (data == NULL) {
    return NULL;
  }
  return ((GmoGimFile *)data)->root;
}
