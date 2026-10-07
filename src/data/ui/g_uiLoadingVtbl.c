// bdc 0x08af47dc g_uiLoadingVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiLoadingVtbl = {
    {0}, { .fn = (void *)UiLoadingDtor }, { .fn = (void *)UiLoadingUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiLoadingDraw },
    { .fn = (void *)UiLoadingSetField }, { .fn = (void *)UiLoadingGetField },
};
