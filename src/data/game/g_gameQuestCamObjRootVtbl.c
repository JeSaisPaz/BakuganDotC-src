// bdc 0x08af6ee0 g_gameQuestCamObjRootVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameQuestCamObjRootVtbl = {
    {0}, { .fn = (void *)GameQuestCamObjVirtualDtor }, { .fn = (void *)CxxPureVirtualCalled },
    { .fn = (void *)GameQuestCamObjSetLookAtNop }, { .fn = (void *)GameQuestCamObjResetNop },
    { .fn = (void *)CxxPureVirtualCalled }, { .fn = (void *)CxxPureVirtualCalled },
};
