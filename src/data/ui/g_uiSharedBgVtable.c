// bdc 0x08af4814 g_uiSharedBgVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiSharedBgVtable = {
    {0}, { .fn = (void *)UiSharedBgDtor }, { .fn = (void *)UiSharedBgUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiSharedBgDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
