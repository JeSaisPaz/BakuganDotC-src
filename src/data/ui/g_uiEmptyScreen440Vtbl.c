// bdc 0x08af48f4 g_uiEmptyScreen440Vtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiEmptyScreen440Vtbl = {
    {0}, { .fn = (void *)UiEmptyScreen440Dtor }, { .fn = (void *)UiScreenUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiScreenDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
