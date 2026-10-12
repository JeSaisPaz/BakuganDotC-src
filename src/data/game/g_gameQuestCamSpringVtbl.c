// bdc 0x08af4554 g_gameQuestCamSpringVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameQuestCamSpringVtbl = {
    {0}, { .fn = (void *)GameQuestCamObjDtor }, { .fn = (void *)CxxPureVirtualCalled },
    { .fn = (void *)GameQuestCamObjSetLookAtNop }, { .fn = (void *)GameQuestCamObjResetNop },
    { .fn = (void *)CxxPureVirtualCalled }, { .fn = (void *)CxxPureVirtualCalled },
};
