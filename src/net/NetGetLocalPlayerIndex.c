// bdc 0x0880d32c NetGetLocalPlayerIndex
#include "bdc.h"

/* Returns `g_netLocalPlayerIndex` (singleton accessor, named by `bdc singleton`). */

s32 NetGetLocalPlayerIndex(void)

{
  return g_netLocalPlayerIndex;
}

