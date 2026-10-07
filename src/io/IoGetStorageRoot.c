// bdc 0x089fb574 IoGetStorageRoot
#include "bdc.h"

/* Returns `g_ioStorageRoot` (singleton accessor, named by `bdc singleton`). */

s32 IoGetStorageRoot(void)

{
  return g_ioStorageRoot;
}

