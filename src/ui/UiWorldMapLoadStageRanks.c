// bdc 0x08996434 UiWorldMapLoadStageRanks
#include "bdc.h"

/* Clears the stage tables of `UiWorldMap` (`+0x110e..+0x11cb`) and, in rank mode
   (`UiWorldMapIsRankMode`), fills them from the profile: the 40 stage records (4 slots per area)
   copied from `stageRecords` with the rank masked to its low 7 bits; per area (10) the stage count
   `stageCount` (3 for areas 0..4, 1 for 5..9), the number of cleared stages `clearedStage` (bits
   `area * 4 + 0..2` of `stageCleared`), and `newRankFlag` for areas whose used slots are all cleared
   (3 of 3 for areas 0..4, at least 1 for 5..9) but still have rank 0. */

void UiWorldMapLoadStageRanks(UiScreen *screen)
{
    UiWorldMap *map = (UiWorldMap *)screen;
    int i;
    int j;

    memset(map->stageRecords, 0, 0xbe);
    if (!UiWorldMapIsRankMode(screen)) {
        return;
    }

    for (i = 0; i < 40; i++) {
        u8 area = (u8)(i / 4);
        u8 slot = (u8)(i % 4);

        map->stageRecords[i].id = SaveGetProfile()->data->stageRecords[area * 4 + slot].id;
        map->stageRecords[i].rank = SaveGetProfile()->data->stageRecords[area * 4 + slot].rank & 0x7f;
        map->stageRecords[i].score = SaveGetProfile()->data->stageRecords[area * 4 + slot].score;
    }

    for (i = 0; i < 10; i++) {
        map->stageCount[i] = (i < 5) ? 3 : 1;
    }

    for (i = 0; i < 10; i++) {
        u8 cleared = 0;

        for (j = 0; j < 3; j++) {
            int bit = (u8)i * 4 + (u8)j;

            if ((u8)(SaveGetProfile()->data->stageCleared[bit / 8] & (1 << (bit % 8))) != 0) {
                cleared++;
            }
        }
        map->clearedStage[i] = cleared;
    }

    for (i = 0; i < 10; i++) {
        if (i < 5) {
            if (map->clearedStage[i] >= 3) {
                u8 unranked = 0;

                for (j = 0; j < 3; j++) {
                    if (map->stageRecords[i * 4 + j].rank == 0) {
                        unranked++;
                    }
                }
                if (unranked == 3) {
                    map->newRankFlag[i] = 1;
                }
            }
        } else if (map->clearedStage[i] != 0 && map->stageRecords[i * 4].rank == 0) {
            map->newRankFlag[i] = 1;
        }
    }
}
