// bdc 0x08af508c g_uiEmptyScreen365Vtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiEmptyScreen365Vtbl = {
    {0}, { .fn = (void *)UiEmptyScreen365Dtor }, { .fn = (void *)UiScreenUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiScreenDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
