// bdc 0x0889d9c4 BtlStageSetFromEventId
#include "bdc.h"

/* When script global 8 (battle rule mode) is 1 (battle started from an event), recomputes the
   stage number (script global 1) from the battle event id in script global 9:
   `(id / 100000) * 4 + (id / 10000) % 10`, where `id / 100000` is the field folder index
   (`F0Japan`..`F6Core`) used by `BtlStageLoadMap`. Otherwise leaves the stage untouched. */
void BtlStageSetFromEventId(void)
{
    s32 *vars = g_scriptGlobalVars;

    if (vars[8] == 1) {
        s32 eventId = vars[9];
        vars[1] = (eventId / 100000) * 4 + (eventId / 10000) % 10;
    }
}
