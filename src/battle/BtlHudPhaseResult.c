// bdc 0x0884470c BtlHudPhaseResult
#include "bdc.h"

/* HUD phase 9: only runs `BtlHudUpdateResultScreen`. */

void BtlHudPhaseResult(BtlHud *self)
{
    BtlHudUpdateResultScreen(self);
}
