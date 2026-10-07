// bdc 0x0880d2bc SaveGetDataBlock
#include "bdc.h"

/* Returns `g_saveDataBlock` (singleton accessor, named by `bdc singleton`). */

void *SaveGetDataBlock(void)

{
  return g_saveDataBlock;
}

