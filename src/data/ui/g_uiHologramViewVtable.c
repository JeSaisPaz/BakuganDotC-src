// bdc 0x08af4a44 g_uiHologramViewVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiHologramViewVtable = {
    {0}, { .fn = (void *)UiHologramViewDtor }, { .fn = (void *)UiHologramViewUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiHologramViewDraw },
    { .fn = (void *)UiHologramViewSetField }, { .fn = (void *)UiHologramViewGetField },
};
