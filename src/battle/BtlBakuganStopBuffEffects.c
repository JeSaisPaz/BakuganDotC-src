// bdc 0x08860220 BtlBakuganStopBuffEffects
#include "bdc.h"

/* Stops the effects 0x57..0x5a, 0x167 and 0x166 (in that order) attached to the unit's pelvis
   anchor position `anchorMatrix[3]` (`+0x250`) on `g_worldEffectMgr`
   (`GfxEffectStopAttached`). */
void BtlBakuganStopBuffEffects(BtlBakugan *self)
{
    float *attach = self->anchorMatrix[3];

    GfxEffectStopAttached(g_worldEffectMgr, 0x57, attach);
    GfxEffectStopAttached(g_worldEffectMgr, 0x58, attach);
    GfxEffectStopAttached(g_worldEffectMgr, 0x59, attach);
    GfxEffectStopAttached(g_worldEffectMgr, 0x5a, attach);
    GfxEffectStopAttached(g_worldEffectMgr, 0x167, attach);
    GfxEffectStopAttached(g_worldEffectMgr, 0x166, attach);
}
