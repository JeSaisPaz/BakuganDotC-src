// bdc 0x08af492c g_uiEmptyScreen450Vtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiEmptyScreen450Vtbl = {
    {0}, { .fn = (void *)UiEmptyScreen450Dtor }, { .fn = (void *)UiScreenUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiScreenDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
