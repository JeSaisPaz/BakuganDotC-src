// bdc 0x08af4ecc g_uiCardEquipVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiCardEquipVtbl = {
    {0}, { .fn = (void *)UiCardEquipDtor }, { .fn = (void *)UiCardEquipUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiCardEquipDraw },
    { .fn = (void *)UiCardEquipSetField }, { .fn = (void *)UiCardEquipGetField },
};
