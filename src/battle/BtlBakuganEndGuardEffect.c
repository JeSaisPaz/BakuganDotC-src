// bdc 0x088655c8 BtlBakuganEndGuardEffect
#include "bdc.h"

/* Ends the guard shield effect of `BtlBakuganStartGuardEffect`: only when `guardEffect` (`+0x394`)
   is 0x10, sets it to 0xf (from there `BtlBakuganUpdateStatusVisuals` counts it down) and
   switches the effects attached to `guardAnchor` (`+0x300`) to state 2 (fade out) with
   `GfxEffectSetStateAttached`: ids 0x5e/0x11f on `g_btlUnitEffectMgr` for Bakugan kind 0x15
   (`+0x8`), otherwise ids 0x163/0x164 on `g_worldEffectMgr`. */

void BtlBakuganEndGuardEffect(BtlBakugan *self)
{
    float *anchor;

    if (self->guardEffect != 0x10) {
        return;
    }
    self->guardEffect = 0xf;
    anchor = self->guardAnchor;
    if (self->base.base.unk08 == 0x15) {
        GfxEffectSetStateAttached(g_btlUnitEffectMgr, 0x5e, anchor, 2);
        GfxEffectSetStateAttached(g_btlUnitEffectMgr, 0x11f, anchor, 2);
    } else {
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x163, anchor, 2);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x164, anchor, 2);
    }
}
