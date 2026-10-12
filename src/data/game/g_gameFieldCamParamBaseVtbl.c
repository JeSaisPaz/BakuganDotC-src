// bdc 0x08af6dc0 g_gameFieldCamParamBaseVtbl
#include "bdc.h"

__typeof__(VtblEntry[4]) g_gameFieldCamParamBaseVtbl = {
    {0}, { .fn = (void *)GameFieldCamParamBaseDtor }, { .fn = (void *)CxxPureVirtualCalled },
    { .fn = (void *)GameFieldCamParamGet },
};
