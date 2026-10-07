// bdc 0x08af4cd4 g_uiEmptyScreen400Vtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiEmptyScreen400Vtbl = {
    {0}, { .fn = (void *)UiEmptyScreen400Dtor }, { .fn = (void *)UiScreenUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiScreenDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
