// bdc 0x088a841c BtlItemSpawnerGetDelay
#include "bdc.h"

/* Returns the respawn delay (frames) of an item spawner: copies the delay table
   `g_btlItemSpawnDelays` to the stack and, by the battle time in profile word 2
   (`SaveProfileGetWord`; -1 = timer off, then the base is 900 frames), takes the first row whose
   `minSeconds * 60` the time reaches (the fourth row when none of the first three does);
   `first` selects its `firstDelay` over `delay` (x30 frames). Adds 30 frames per spawner `index`
   and a random 0..4 x 30 frames (`CoreRandNext`). */
int BtlItemSpawnerGetDelay(BtlItemSpawner *spawner, char first)
{
    BtlItemSpawnDelayRow rows[4];
    const BtlItemSpawnDelayRow *row;
    s32 time;
    int delay;
    int i;

    time = (s32)SaveProfileGetWord(SaveGetProfile(), 2);
    memcpy(rows, g_btlItemSpawnDelays, sizeof(rows));
    delay = 900;
    if (time != -1) {
        for (i = 0; i < 3; i++) {
            if (!(time < rows[i].minSeconds * 60)) {
                break;
            }
        }
        row = &rows[i];
        delay = (first != 0 ? row->firstDelay : row->delay) * 30;
    }
    return delay + spawner->index * 30 + (int)CoreRandNext(5) * 30;
}
