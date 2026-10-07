// bdc 0x08a91a64 g_gameFieldCameraModeFns
#include "bdc.h"

__typeof__(MemberFnPtr[10]) g_gameFieldCameraModeFns = {
    { .pfn = (void *)GameFieldCameraModeFollow }, { .pfn = (void *)GameFieldCameraModeTalk },
    { .pfn = (void *)GameFieldCameraModeTerminal }, { .pfn = (void *)GameFieldCameraFollowStep },
    { .pfn = (void *)GameFieldCameraModeAim }, { .pfn = (void *)GameFieldCameraModeHold },
    { .pfn = (void *)GameFieldCameraModeBlend }, { .pfn = (void *)GameFieldCameraModeOrbit },
    { .pfn = (void *)GameFieldCameraModeHeadView }, { .pfn = (void *)GameFieldCameraMode9 },
};
