// bdc 0x089bec98 CoreRandResetSeed
#include "bdc.h"

/* Resets the software random generator `g_randState` to its fixed default seed `8d256309 ae5be5b5
   c4be4186 1ad7bde3`. Called once by `BootMainThread` during start-up and again by the four battle
   main-task functions `BtlMainTaskCtor`, `BtlMainPhaseLoad`, `BtlMainPhaseBattle` and
   `BtlMainTeardown` (all five also run `CoreRandLoadVfpuState`). */
void CoreRandResetSeed(void)
{
    g_randState[0] = 0x8d256309;
    g_randState[1] = 0xae5be5b5;
    g_randState[2] = 0xc4be4186;
    g_randState[3] = 0x1ad7bde3;
}
