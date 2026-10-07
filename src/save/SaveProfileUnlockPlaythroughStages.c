// bdc 0x089b2980 SaveProfileUnlockPlaythroughStages
#include "bdc.h"

/* Start-of-play-through unlocks: marks the stages `0x14` and `0x18` as cleared in the player
   profile exactly like `SaveProfileMarkStageCleared` does for a won stage (area bit `stage / 4`
   in `areaCleared`, bit `area * 4 + stage % 4` in `stageCleared`), then grants the play-through
   bonus (`SaveProfileGrantPlaythroughBonus`). Called only by `ScriptOpStartNextPlaythrough`
   after it bumped the play-through counter `playthrough`. */

void SaveProfileUnlockPlaythroughStages(void)
{
    u8 stages[3];
    s32 i;
    s32 stage;
    s32 area;
    s32 bit;
    SaveProfileData *data;

    stages[0] = 0x14;
    stages[1] = 0x18;
    stages[2] = 0xff;
    stage = stages[0];
    i = 0;
    while (stage != 0xff) {
        data = SaveGetProfile()->data;
        area = (u8)(stage / 4);
        data->areaCleared[area / 8] |= 1 << (area % 8);
        data = SaveGetProfile()->data;
        bit = area * 4 + (u8)(stage % 4);
        data->stageCleared[bit / 8] |= 1 << (bit % 8);
        i++;
        stage = stages[i];
    }
    SaveProfileGrantPlaythroughBonus();
}
