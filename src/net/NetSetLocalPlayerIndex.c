// bdc 0x0880d320 NetSetLocalPlayerIndex
#include "bdc.h"

/* Stores its argument in `g_netLocalPlayerIndex` (singleton accessor, named by `bdc singleton`).
    */

void NetSetLocalPlayerIndex(s32 value)

{
  g_netLocalPlayerIndex = value;
  return;
}

