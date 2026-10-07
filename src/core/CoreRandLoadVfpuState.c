// bdc 0x089bec48 CoreRandLoadVfpuState
#include "bdc.h"

/* Loads the eight words of `g_vfpuRandState` into the VFPU control registers `RCX0..RCX7`,
   restoring the seed of the hardware random generator behind the `vrnd*` instructions. Called once
   by `BootMainThread` at start-up, right before `CoreRandResetSeed`, and with it at the start of
   netplay sessions. The `lw`/`mtvc` pairs lift to the platform hook `PlatformRandLoadState`. */
void CoreRandLoadVfpuState(void)
{
    PlatformRandLoadState(g_vfpuRandState);
}
