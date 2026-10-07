// bdc 0x089d011c NetModeFlagClear
#include "bdc.h"

/* Clears `g_netModeFlag`. Called by `BtlMainTaskCtor` and `BtlMainTeardown`. */

void NetModeFlagClear(void)

{
  g_netModeFlag = '\0';
  return;
}

