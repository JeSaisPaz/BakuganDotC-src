// bdc 0x08a198ac GmoModelTryLoad
#include "bdc.h"

/* Returns whether `GmoModelLoad` succeeded (non-zero result). */

bool GmoModelTryLoad(GmoModel *self, const void *gmo, u32 size, s32 index)

{
  return GmoModelLoad(self, gmo, size, index) != 0;
}

