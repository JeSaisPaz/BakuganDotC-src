// bdc 0x08af51a4 g_uiEmptyScreen360Vtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiEmptyScreen360Vtbl = {
    {0}, { .fn = (void *)UiEmptyScreen360Dtor }, { .fn = (void *)UiScreenUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiScreenDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
