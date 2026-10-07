// bdc 0x08af51dc g_uiComboListVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiComboListVtbl = {
    {0}, { .fn = (void *)UiComboListDtor }, { .fn = (void *)UiComboListUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiComboListDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
