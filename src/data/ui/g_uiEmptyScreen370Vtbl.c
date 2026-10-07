// bdc 0x08af4e24 g_uiEmptyScreen370Vtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiEmptyScreen370Vtbl = {
    {0}, { .fn = (void *)UiEmptyScreen370Dtor }, { .fn = (void *)UiScreenUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiScreenDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
