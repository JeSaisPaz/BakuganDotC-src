// bdc 0x08af4f04 g_uiOptionVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiOptionVtbl = {
    {0}, { .fn = (void *)UiOptionDtor }, { .fn = (void *)UiOptionUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiOptionDraw },
    { .fn = (void *)UiOptionSetField }, { .fn = (void *)UiOptionGetField },
};
