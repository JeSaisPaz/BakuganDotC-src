// bdc 0x0882ba24 ActorCrystalSpawnTaskStep
#include "bdc.h"

/* Step function of the crystal spawn task (id 105, `ActorCrystalSpawnTaskUpdate`), a state
   machine on `+0x10` that needs a player unit (`BtlGetPlayerBakugan`): state 0, once the request
   byte `0x08ab9f70` is set, counts the crystal points (`ActorStageObjRecordCountType4`), spawns
   one crystal at a random free point (`ActorCrystalPickFreeSpawnPoint` →
   `ActorCrystalSpawnAtPoint`) and one crystal stand per point
   (`ActorCrystalStandSpawnAtPoint`), then goes to 10; state 10 jumps to 12 once
   `BtlIsTimeRunningOut` reports true; state 11 advances to 12 unconditionally. */

void ActorCrystalSpawnTaskStep(CoreTask *task)
{
    ActorCrystalSpawnTask *self = (ActorCrystalSpawnTask *)task;
    s32 count;
    s32 i;

    if (BtlGetPlayerBakugan() == NULL)
        return;
    if (self->step < 10) {
        if (self->step != 0)
            return;
        if (g_actorCrystalSpawnRequest == 0)
            return;
        count = ActorStageObjRecordCountType4();
        if (count > 0) {
            ActorCrystalSpawnAtPoint(ActorCrystalPickFreeSpawnPoint());
            for (i = 0; i < count; i++)
                ActorCrystalStandSpawnAtPoint(i);
        }
        self->step = 10;
    } else if (self->step > 10) {
        if (self->step < 12)
            self->step = self->step + 1;
        return;
    }
    if (!BtlIsTimeRunningOut())
        return;
    self->step = self->step + 1;
    self->step = self->step + 1;
}
