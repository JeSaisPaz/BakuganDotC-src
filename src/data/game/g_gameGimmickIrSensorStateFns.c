// bdc 0x08a96ac8 g_gameGimmickIrSensorStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[3]) g_gameGimmickIrSensorStateFns = {
    { .pfn = (void *)GameGimmickIrSensorBeamState },
    { .pfn = (void *)GameGimmickIrSensorCooldownState },
    { .pfn = (void *)GameGimmickIrSensorAdvanceStepState },
};
