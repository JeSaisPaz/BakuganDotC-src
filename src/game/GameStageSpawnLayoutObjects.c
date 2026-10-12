// bdc 0x088d3f98 GameStageSpawnLayoutObjects
#include "bdc.h"

/* Spawns every record of layout table 1 of the current stage (`GameStageGetLayoutTable`; 12-byte
   `BtlStagePlacement` `{s16 x, y, z, heading, id, ?}`) through `ActorStageObjRecordAdd(pos, id & 0x3ff, index)`. */

void GameStageSpawnLayoutObjects(void)
{
    BtlStagePlacementList *table;
    BtlStagePlacement *rec;
    s32 count;
    s32 i;
    float pos[4] __attribute__((aligned(16)));

    table = GameStageGetLayoutTable(1);
    count = table->count;
    rec = table->records;
    for (i = 0; i < count; i++) {
        pos[0] = (float)rec->pos[0];
        pos[1] = (float)rec->pos[1];
        pos[2] = (float)rec->pos[2];
        pos[3] = (float)rec->pos[3];
        ActorStageObjRecordAdd(pos, (s16)rec->kindArg & 0x3ff, (s16)i, 0, 0);
        rec++;
    }
}
