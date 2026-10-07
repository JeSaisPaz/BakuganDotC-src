// bdc 0x08865670 BtlBakuganStopStatusEffects
#include "bdc.h"

/* Ends the visual effects of the Bakugan's status `status` on g_worldEffectMgr. Status 1: stops
   sound 0x2001f5 for the local player (BtlBakuganIsLocalPlayer) and sets effects 0x232..0x234 at
   the anchor position (anchorMatrix row 3) to state 2. Status 2: effects 0x236/0x237 at the anchor
   to state 2. Status 3: effect 0x29b at pos to state 2. Statuses 4/5/6 stop effect 0x53/0x55/0x54
   at pos and set 0x299 there to state 2; status 7 does all three. Status 0 does everything
   (sound, 0x29b, 0x232..0x234, 0x53/0x55/0x54 with 0x299, 0x236/0x237). */

void BtlBakuganStopStatusEffects(BtlBakugan *self, int status)
{
    float *anchor;
    float *pos;

    if (status == 0) {
        anchor = self->anchorMatrix[3];
        if (BtlBakuganIsLocalPlayer(self)) {
            BtlBakuganStopSound(self, 0x2001f5);
        }
        pos = self->base.pos;
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x29b, pos, 2);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x232, anchor, 2);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x233, anchor, 2);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x234, anchor, 2);
        GfxEffectStopAttached(g_worldEffectMgr, 0x53, pos);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x299, pos, 2);
        GfxEffectStopAttached(g_worldEffectMgr, 0x55, pos);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x299, pos, 2);
        GfxEffectStopAttached(g_worldEffectMgr, 0x54, pos);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x299, pos, 2);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x236, anchor, 2);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x237, anchor, 2);
        return;
    }
    if (status == 1) {
        anchor = self->anchorMatrix[3];
        if (BtlBakuganIsLocalPlayer(self)) {
            BtlBakuganStopSound(self, 0x2001f5);
        }
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x232, anchor, 2);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x233, anchor, 2);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x234, anchor, 2);
    }
    if (status == 2) {
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x236, self->anchorMatrix[3], 2);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x237, self->anchorMatrix[3], 2);
    }
    if (status == 3) {
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x29b, self->base.pos, 2);
    }
    if (status == 4 || status == 7) {
        pos = self->base.pos;
        GfxEffectStopAttached(g_worldEffectMgr, 0x53, pos);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x299, pos, 2);
    }
    if (status == 5 || status == 7) {
        pos = self->base.pos;
        GfxEffectStopAttached(g_worldEffectMgr, 0x55, pos);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x299, pos, 2);
    }
    if (status == 6 || status == 7) {
        pos = self->base.pos;
        GfxEffectStopAttached(g_worldEffectMgr, 0x54, pos);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x299, pos, 2);
    }
}
