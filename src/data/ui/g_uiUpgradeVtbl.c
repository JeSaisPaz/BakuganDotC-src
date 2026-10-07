// bdc 0x08af499c g_uiUpgradeVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiUpgradeVtbl = {
    {0}, { .fn = (void *)UiUpgradeDtor }, { .fn = (void *)UiUpgradeUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiUpgradeDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
