// bdc 0x088659b4 BtlBakuganStopAllStatusEffects
#include "bdc.h"

/* Clears every status visual of a unit: ends the visuals of status 0
   (`BtlBakuganStopStatusEffects`), sets the effects 0x49 and 0x1c5 attached to the pelvis anchor
   position `anchorMatrix[3]` (`+0x250`) to state 2 on `g_worldEffectMgr`
   (`GfxEffectSetStateAttached`) and stops the buff effects (`BtlBakuganStopBuffEffects`). */

void BtlBakuganStopAllStatusEffects(BtlBakugan *self)
{
    BtlBakuganStopStatusEffects(self, 0);
    GfxEffectSetStateAttached(g_worldEffectMgr, 0x49, self->anchorMatrix[3], 2);
    GfxEffectSetStateAttached(g_worldEffectMgr, 0x1c5, self->anchorMatrix[3], 2);
    BtlBakuganStopBuffEffects(self);
}
