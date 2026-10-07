// bdc 0x088d3f98 GameStageSpawnLayoutObjects
#include "bdc.h"

/* Spawns every record of layout table 1 of the current stage (`GameStageGetLayoutTable`; 12-byte
   `{s16 x, y, z, heading, id, ?}`) through `ActorStageObjRecordAdd(pos, id & 0x3ff, index)`. */

void GameStageSpawnLayoutObjects(void)
{
    s32 *table;
    s16 *rec;
    s32 count;
    s32 i;
    float pos[4] __attribute__((aligned(16)));

    table = GameStageGetLayoutTable(1);
    count = table[0];
    rec = *(s16 **)(table + 1);
    for (i = 0; i < count; i++) {
        pos[0] = (float)rec[0];
        pos[1] = (float)rec[1];
        pos[2] = (float)rec[2];
        pos[3] = (float)rec[3];
        ActorStageObjRecordAdd(pos, rec[4] & 0x3ff, (s16)i, 0, 0);
        rec += 6;
    }
}
