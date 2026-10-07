// bdc 0x0889d36c BtlStageSpawnPlacedObjects
#include "bdc.h"

/* Spawns the current arena's placed objects from placement list 0 (`BtlStageGetPlacementList`):
   for each 12-byte `BtlStagePlacement` accepted by `ActorStageObjLayoutEntryShouldSpawn` it
   converts the s16 position/yaw quadruple to floats and creates a layout record
   (`ActorStageObjRecordAdd`) with kind = `kindArg` bits 0..9, arg = bits 10..15,
   type = `typeGroup` bits 2..7 and variant = `variant` bits 0..5; then runs one layout step
   (`ActorStageObjRecordUpdateAll`). Called by `BtlStageLoadMap`. */
void BtlStageSpawnPlacedObjects(void)
{
    BtlStagePlacementList *list;
    BtlStagePlacement *entry;
    s32 count;
    s32 i;
    float pos[4] __attribute__((aligned(16)));

    list = BtlStageGetPlacementList(0);
    count = list->count;
    entry = list->records;
    for (i = 0; i < count; i++, entry++) {
        if (!ActorStageObjLayoutEntryShouldSpawn(entry)) {
            continue;
        }
        pos[0] = (float)entry->pos[0];
        pos[1] = (float)entry->pos[1];
        pos[2] = (float)entry->pos[2];
        pos[3] = (float)entry->pos[3];
        ActorStageObjRecordAdd(pos, entry->kindArg & 0x3ff, (entry->kindArg & 0xfc00) >> 10,
                               (entry->typeGroup & 0xfc) >> 2, entry->variant & 0x3f);
    }
    ActorStageObjRecordUpdateAll();
}
