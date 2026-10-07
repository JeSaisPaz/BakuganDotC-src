// bdc 0x08878dd4 BtlAttackStopSeekerEffects
#include "bdc.h"

/* Stops the attached effects with definition ids 0x1cb, 0x1cc and 0x1cd (in that order) whose
   attach pointer is `anchor` on effect manager `mgr` (`GfxEffectStopAttached`; results ignored).
   Used by `BtlAttackUpdateSeekerBolt` and `BtlAttackType7CUpdate`. */
void BtlAttackStopSeekerEffects(void *mgr, void *anchor)
{
    GfxEffectStopAttached(mgr, 0x1cb, anchor);
    GfxEffectStopAttached(mgr, 0x1cc, anchor);
    GfxEffectStopAttached(mgr, 0x1cd, anchor);
}
