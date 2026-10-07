// bdc 0x08af13b4 g_uiNameEntryVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiNameEntryVtbl = {
    {0}, { .fn = (void *)UiNameEntryDtor }, { .fn = (void *)UiNameEntryUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiNameEntryDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
