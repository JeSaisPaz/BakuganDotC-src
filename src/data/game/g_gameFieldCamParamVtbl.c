// bdc 0x08af6de0 g_gameFieldCamParamVtbl
#include "bdc.h"

__typeof__(VtblEntry[4]) g_gameFieldCamParamVtbl = {
    {0}, { .fn = (void *)GameFieldCamParamDtor }, { .fn = (void *)GameFieldCamParamGetPayload },
    { .fn = (void *)GameFieldCamParamGet },
};
