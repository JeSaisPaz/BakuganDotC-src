// bdc 0x08af4e94 g_uiEquipVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiEquipVtbl = {
    {0}, { .fn = (void *)UiEquipDtor }, { .fn = (void *)UiEquipUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiEquipDraw },
    { .fn = (void *)UiEquipSetField }, { .fn = (void *)UiEquipGetField },
};
