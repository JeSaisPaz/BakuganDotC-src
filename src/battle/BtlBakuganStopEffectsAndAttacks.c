// bdc 0x088601cc BtlBakuganStopEffectsAndAttacks
#include "bdc.h"

/* Stops every effect (any id) attached to the unit's effect anchor `effectAnchor` and to the
   translation row of `anchorMatrix` on `g_worldEffectMgr` (`GfxEffectStopAttached`), then ends
   the sustained attacks the unit owns (`BtlAttackEndOwnedSustained`). */

void BtlBakuganStopEffectsAndAttacks(BtlBakugan *self)
{
    GfxEffectStopAttached(g_worldEffectMgr, -1, self->effectAnchor);
    GfxEffectStopAttached(g_worldEffectMgr, -1, self->anchorMatrix[3]);
    BtlAttackEndOwnedSustained(self);
}
