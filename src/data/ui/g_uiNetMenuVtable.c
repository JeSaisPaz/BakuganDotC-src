// bdc 0x08af4d7c g_uiNetMenuVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiNetMenuVtable = {
    {0}, { .fn = (void *)UiNetMenuDtor }, { .fn = (void *)UiNetMenuUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiNetMenuDraw },
    { .fn = (void *)UiNetMenuSetField }, { .fn = (void *)UiNetMenuGetField },
};
