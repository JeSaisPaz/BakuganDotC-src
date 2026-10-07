// bdc 0x08a96b8c g_gameGimmickCameraStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[3]) g_gameGimmickCameraStateFns = {
    { .pfn = (void *)GameGimmickCameraScanState },
    { .pfn = (void *)GameGimmickCameraCooldownState },
    { .pfn = (void *)GameGimmickCameraDecayState },
};
