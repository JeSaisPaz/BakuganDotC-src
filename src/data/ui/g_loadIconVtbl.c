// bdc 0x08af145c g_loadIconVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_loadIconVtbl = {
    {0}, { .fn = (void *)UiLoadIconDtor }, { .fn = (void *)UiLoadIconUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiLoadIconDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
