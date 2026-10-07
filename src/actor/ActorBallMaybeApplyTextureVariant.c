// bdc 0x088b9378 ActorBallMaybeApplyTextureVariant
#include "bdc.h"

/* Applies a numbered texture variant (`ActorBallApplyTextureVariant`) only when the battle camera
   task exists (`BtlCameraTaskExists`) and either global script variable 8 equals 1 or profile
   flag 0 is set (`SaveGetProfileFlag0`); returns the result or 0. */

u32 ActorBallMaybeApplyTextureVariant(void *model, s32 variant)
{
    bool apply = false;
    u32 result = 0;

    if (BtlCameraTaskExists() != 0) {
        if (g_scriptGlobalVars[8] == 1) {
            apply = true;
        } else if (SaveGetProfileFlag0() != 0) {
            apply = true;
        }
    }
    if (apply) {
        result = ActorBallApplyTextureVariant(model, variant);
    }
    return result;
}
