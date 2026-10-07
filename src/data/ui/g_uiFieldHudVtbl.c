// bdc 0x08af2df4 g_uiFieldHudVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiFieldHudVtbl = {
    {0}, { .fn = (void *)UiFieldHudDtor }, { .fn = (void *)UiFieldHudUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiFieldHudDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
