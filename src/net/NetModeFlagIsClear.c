// bdc 0x089d0148 NetModeFlagIsClear
#include "bdc.h"

/* Returns whether `g_netModeFlag` is 0. Used by `NetPlayUpdate`, `NetPlayLateUpdate` and
   `NetCharaPushMessage`. */
bool NetModeFlagIsClear(void)
{
    return g_netModeFlag == 0;
}
