// bdc 0x089063ac BtlDemoSceneGetMotionId
#include "bdc.h"

/* Returns motion id `index` of the scene player's demo: row 0 of `g_btlDemoMotionIdTables` when
   the demo id is variant 3 (`BtlDemoIdIsVariant3`), row 1 when variant 1
   (`BtlDemoIdIsVariant1`), row 2 otherwise. Only the upper bound is checked (`index < 3`); an
   index >= 3 or a -1 entry gives the default motion 0x125. */
s32 BtlDemoSceneGetMotionId(BtlDemoScenePlayer *player, s32 index)
{
    s32 motion = -1;
    s32 row = 0;

    if (!BtlDemoIdIsVariant3(player->demoId)) {
        row = 2;
        if (BtlDemoIdIsVariant1(player->demoId)) {
            row = 1;
        }
    }
    if (index < 3) {
        motion = g_btlDemoMotionIdTables[row][index];
    }
    if (motion == -1) {
        motion = 0x125;
    }
    return motion;
}
