// bdc 0x0884c400 BtlIsTimeRunningOut
#include "bdc.h"

/* True when the battle rule mode (script global variable 8) is 2 and the remaining battle
   time (profile word 2, in frames) is in 0..0x708 (at most 1800 frames, the last 30 s at
   60 fps). */

bool BtlIsTimeRunningOut(void)
{
    s32 framesLeft;
    bool result;

    result = false;
    if (g_scriptGlobalVars[8] == 2) {
        framesLeft = (s32)SaveProfileGetWord((SaveProfile *)SaveGetProfile(), 2);
        if (framesLeft >= 0 && framesLeft < 0x709) {
            result = true;
        }
    }
    return result;
}
