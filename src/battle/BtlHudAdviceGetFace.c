// bdc 0x08837c60 BtlHudAdviceGetFace
#include "bdc.h"

/* Returns the face index of the battle advisor shown in HUD advice messages:
   the per-stage table entry for the current stage number (script global
   variable 1), or, when the battle rule mode (script global variable 8) is 2,
   the active team Bakugan's talk face minus 0x23. -1 = no advisor. */
int BtlHudAdviceGetFace(void)
{
    if (g_scriptGlobalVars[8] == 2) {
        return SaveGetTeamBakuganFace(-1) - 0x23;
    }
    return g_btlAdvisorFaceByStage[g_scriptGlobalVars[1]];
}
