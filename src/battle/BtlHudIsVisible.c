// bdc 0x0882e488 BtlHudIsVisible
#include "bdc.h"

/* True while the HUD-hidden byte `g_btlHudHidden` is clear. Checked by
   `BtlHudUpdateTargetMarker`, `BtlHudDrawLayers` and other HUD widgets. */
bool BtlHudIsVisible(void)
{
    return g_btlHudHidden == 0;
}
