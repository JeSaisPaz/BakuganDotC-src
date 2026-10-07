// bdc 0x088a8664 BtlItemSpawnerStopEffects
#include "bdc.h"

/* Stops all item-point effects (ids 0x60, 0x123..0x126) on the battle unit effect manager
   g_btlUnitEffectMgr (`GfxEffectStopAttached` with any attach pointer). */

void BtlItemSpawnerStopEffects(void)
{
    GfxEffectStopAttached(g_btlUnitEffectMgr, 0x60, NULL);
    GfxEffectStopAttached(g_btlUnitEffectMgr, 0x123, NULL);
    GfxEffectStopAttached(g_btlUnitEffectMgr, 0x124, NULL);
    GfxEffectStopAttached(g_btlUnitEffectMgr, 0x125, NULL);
    GfxEffectStopAttached(g_btlUnitEffectMgr, 0x126, NULL);
}
