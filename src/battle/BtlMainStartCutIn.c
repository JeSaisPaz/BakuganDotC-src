// bdc 0x0884de2c BtlMainStartCutIn
#include "bdc.h"

/* Starts the battle cut-in for `unit` unless task 0x1e1 already exists (`CoreTaskExists`): the
   unit's target (`BtlBakuganGetTarget`) must be a BtlUnitAlt (vtable slot 12,
   `BtlBakuganIsUnitAlt`) or a Bakugan (slot 10, `BtlBakuganIsBakugan`), and `unit` must be
   player-controlled (`isPlayer`). Then clears `g_btlHudHidden`, flags the targeted stage objects
   (`ActorStageObjMarkTargeted`), creates task 0x1e1 (`CoreTaskCreateDefault`, the cut-in
   task), sets both `phase` and `drawPhase` to 5 and suspends the stage event script
   (`BtlStageSuspendEventScript`). Returns 1 when started, else 0. */
s32 BtlMainStartCutIn(BtlMain *self, BtlBakugan *unit)
{
    BtlBakugan *target;
    const VtblEntry *vtbl;

    if (CoreTaskExists(0x1e1) != 0) {
        return 0;
    }
    target = BtlBakuganGetTarget(unit);
    vtbl = (const VtblEntry *)target->base.base.vtable;
    if (((int (*)(void *))vtbl[12].fn)((u8 *)target + vtbl[12].delta) == 0) {
        target = BtlBakuganGetTarget(unit);
        vtbl = (const VtblEntry *)target->base.base.vtable;
        if (((int (*)(void *))vtbl[10].fn)((u8 *)target + vtbl[10].delta) == 0) {
            return 0;
        }
    }
    if (unit->isPlayer == 0) {
        return 0;
    }
    g_btlHudHidden = 0;
    ActorStageObjMarkTargeted(unit);
    CoreTaskCreateDefault(0x1e1, unit);
    self->phase = 5;
    self->drawPhase = 5;
    BtlStageSuspendEventScript();
    return 1;
}
