// bdc 0x0884cf54 BtlMainGetBgmId
#include "bdc.h"

/* BGM track of the battle: `bgmOverride` if it is not -1; otherwise, from the stage number in
   script global variable 1, `stage / 4 + 11` (signed division) for stages below 0x14, 0x14 for
   stages 0x24, 0x26 and 0x27, and 0x10 for every other stage. */
int BtlMainGetBgmId(BtlMain *self)
{
    int stage;

    if (self->bgmOverride != -1) {
        return self->bgmOverride;
    }
    stage = g_scriptGlobalVars[1];
    if (stage < 0x14) {
        return stage / 4 + 11;
    }
    if (stage == 0x24 || stage == 0x26 || stage == 0x27) {
        return 0x14;
    }
    return 0x10;
}
