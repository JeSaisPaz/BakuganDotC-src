// bdc 0x08af49d4 g_uiAdvSelectVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiAdvSelectVtbl = {
    {0}, { .fn = (void *)UiAdvSelectDtor }, { .fn = (void *)UiAdvSelectUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiAdvSelectDraw },
    { .fn = (void *)UiAdvSelectSetField }, { .fn = (void *)UiAdvSelectGetField },
};
