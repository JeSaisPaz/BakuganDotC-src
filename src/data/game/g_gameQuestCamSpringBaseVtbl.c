// bdc 0x08af6e98 g_gameQuestCamSpringBaseVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameQuestCamSpringBaseVtbl = {
    {0}, { .fn = (void *)GameQuestCamLookAtBaseDtor },
    { .fn = (void *)GameQuestCamLookAtBaseGetLookAt },
    { .fn = (void *)GameQuestCamObjSetLookAtNop }, { .fn = (void *)GameQuestCamObjResetNop },
    { .fn = (void *)CxxPureVirtualCalled }, { .fn = (void *)CxxPureVirtualCalled },
};
