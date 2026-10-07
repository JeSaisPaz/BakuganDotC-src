// bdc 0x089daaa0 GmoGetFirstModel
#include "bdc.h"

/* Returns the first model chunk (type 3) of a loaded GMO file: `GmoChunkFind``(gmo + 0x10, 3,
   0)`, where `gmo + 0x10` is the root chunk behind the 16-byte file signature. */

const void *GmoGetFirstModel(const GmoFile *gmo)
{
  return GmoChunkFind(gmo->root, 3, 0);
}
