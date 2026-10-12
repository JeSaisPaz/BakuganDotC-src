// bdc 0x08af48bc g_uiRepairVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiRepairVtable = {
    {0}, { .fn = (void *)UiRepairDtor }, { .fn = (void *)UiRepairUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiRepairDraw },
    { .fn = (void *)UiRepairSetField }, { .fn = (void *)UiRepairGetField },
};
