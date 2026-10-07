// bdc 0x08af484c g_uiEmptyScreen460Vtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiEmptyScreen460Vtbl = {
    {0}, { .fn = (void *)UiEmptyScreen460Dtor }, { .fn = (void *)UiScreenUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiScreenDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
