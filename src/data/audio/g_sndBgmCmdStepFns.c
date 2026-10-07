// bdc 0x08ac5884 g_sndBgmCmdStepFns
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_sndBgmCmdStepFns = {
    {0}, { .pfn = (void *)SndBgmCmdStepPreload }, { .pfn = (void *)SndBgmCmdStepPlayLoaded },
    { .pfn = (void *)SndBgmCmdStepPlay }, { .pfn = (void *)SndBgmCmdStepStop },
};
