// bdc 0x088654e0 BtlBakuganStartGuardEffect
#include "bdc.h"

/* Starts the guard shield effect of a Bakugan unless it is already on (guardEffect == 0x10): sets
   guardEffect to 0x10, resets the guard age (5 when the previous guard effect was still fading,
   i.e. guardEffect was non-zero, else 0) and spawns effect 0x37 attached to guardAnchor, then
   immediately updates its attached effects 0x37/0x163/0x164 (`GfxEffectSpawnAttached`,
   `GfxEffectUpdateAttachedNow`) on `g_worldEffectMgr`; a unit of kind 0x15 uses
   `g_btlUnitEffectMgr` and effects 0x5e/0x11f/0x120 instead. */
void BtlBakuganStartGuardEffect(BtlBakugan *self)
{
    float *attach;

    if (self->guardEffect == 0x10) {
        return;
    }
    attach = self->guardAnchor;
    self->guardAge = (self->guardEffect != 0) ? 5 : 0;
    self->guardEffect = 0x10;
    if (self->base.base.unk08 == 0x15) {
        GfxEffectSpawnAttached(g_btlUnitEffectMgr, 0x5e, attach);
        GfxEffectUpdateAttachedNow(g_btlUnitEffectMgr, 0x5e, attach);
        GfxEffectUpdateAttachedNow(g_btlUnitEffectMgr, 0x11f, attach);
        GfxEffectUpdateAttachedNow(g_btlUnitEffectMgr, 0x120, attach);
    } else {
        GfxEffectSpawnAttached(g_worldEffectMgr, 0x37, attach);
        GfxEffectUpdateAttachedNow(g_worldEffectMgr, 0x37, attach);
        GfxEffectUpdateAttachedNow(g_worldEffectMgr, 0x163, attach);
        GfxEffectUpdateAttachedNow(g_worldEffectMgr, 0x164, attach);
    }
}
