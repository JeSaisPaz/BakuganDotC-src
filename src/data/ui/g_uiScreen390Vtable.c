// bdc 0x08af4b94 g_uiScreen390Vtable
#include "bdc.h"

__typeof__(VtblEntry[5]) g_uiScreen390Vtable = {
    {0}, { .fn = (void *)UiScreen390Dtor }, { .fn = (void *)UiScreen390Update },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiScreen390Draw },
};
