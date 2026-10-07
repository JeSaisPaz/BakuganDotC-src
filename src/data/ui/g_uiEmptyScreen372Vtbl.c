// bdc 0x08af4aec g_uiEmptyScreen372Vtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiEmptyScreen372Vtbl = {
    {0}, { .fn = (void *)UiEmptyScreen372Dtor }, { .fn = (void *)UiScreenUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiScreenDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
