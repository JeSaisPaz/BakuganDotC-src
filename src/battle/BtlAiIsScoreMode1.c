// bdc 0x0888ef88 BtlAiIsScoreMode1
#include "bdc.h"

/* Returns 1 when the battle rule mode (script global variable 8) is 2 and the
   profile's score mode (profile word 7) is 1, else 0. */
s32 BtlAiIsScoreMode1(void)
{
    if (g_scriptGlobalVars[8] == 2) {
        if (SaveProfileGetWord(SaveGetProfile(), 7) == 1) {
            return 1;
        }
    }
    return 0;
}
