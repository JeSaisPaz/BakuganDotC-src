// bdc 0x08af4bcc g_uiEmptyScreen380Vtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiEmptyScreen380Vtbl = {
    {0}, { .fn = (void *)UiEmptyScreen380Dtor }, { .fn = (void *)UiScreenUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiScreenDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
