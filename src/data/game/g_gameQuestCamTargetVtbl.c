// bdc 0x08af6e58 g_gameQuestCamTargetVtbl
#include "bdc.h"

__typeof__(VtblEntry[8]) g_gameQuestCamTargetVtbl = {
    {0}, { .fn = (void *)GameQuestCamTargetDtor }, { .fn = (void *)GameQuestCamTargetGetLookAt },
    { .fn = (void *)GameQuestCamObjSetLookAtNop }, { .fn = (void *)GameQuestCamObjResetNop },
    { .fn = (void *)CxxPureVirtualCalled }, { .fn = (void *)CxxPureVirtualCalled },
    { .fn = (void *)GameQuestCamTargetGetPoint },
};
