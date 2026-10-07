// bdc 0x08af4ab4 g_uiGauntletSetupVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiGauntletSetupVtbl = {
    {0}, { .fn = (void *)UiGauntletSetupDtor }, { .fn = (void *)UiGauntletSetupUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiGauntletSetupDraw },
    { .fn = (void *)UiGauntletSetupSetField }, { .fn = (void *)UiGauntletSetupGetField },
};
