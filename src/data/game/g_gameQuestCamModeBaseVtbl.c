// bdc 0x08af458c g_gameQuestCamModeBaseVtbl
#include "bdc.h"

__typeof__(VtblEntry[8]) g_gameQuestCamModeBaseVtbl = {
    {0}, { .fn = (void *)GameQuestCamModeBaseDtor }, { .fn = (void *)GameQuestCamTargetGetLookAt },
    { .fn = (void *)GameQuestCamObjSetLookAtNop }, { .fn = (void *)GameQuestCamModeResetSmooth },
    { .fn = (void *)CxxPureVirtualCalled }, { .fn = (void *)CxxPureVirtualCalled },
    { .fn = (void *)GameQuestCamTargetGetPoint },
};
