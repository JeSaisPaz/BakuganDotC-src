// bdc 0x08af47a4 g_uiScreenVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiScreenVtable = {
    {0}, { .fn = (void *)UiScreenDtor }, { .fn = (void *)UiScreenUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiScreenDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
