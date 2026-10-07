// bdc 0x08af50fc g_uiMainMenuVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiMainMenuVtbl = {
    {0}, { .fn = (void *)UiMainMenuDtor }, { .fn = (void *)UiMainMenuUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiMainMenuDraw },
    { .fn = (void *)UiMainMenuSetField }, { .fn = (void *)UiMainMenuGetField },
};
