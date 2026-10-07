// bdc 0x08a91ab4 g_gameFieldCameraExitFns
#include "bdc.h"

__typeof__(MemberFnPtr[10]) g_gameFieldCameraExitFns = {
    {0}, {0}, {0}, {0}, { .pfn = (void *)GameFieldCameraExitAim },
    { .pfn = (void *)GameFieldCameraExitHold }, {0}, { .pfn = (void *)GameFieldCameraExitOrbit },
    { .pfn = (void *)GameFieldCameraExitHeadView }, { .pfn = (void *)GameFieldCameraExitMode9 },
};
