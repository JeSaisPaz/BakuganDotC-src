// bdc 0x08af17c4 g_btlHudVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_btlHudVtbl = {
    {0}, { .fn = (void *)BtlHudDtor }, { .fn = (void *)BtlHudUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)BtlHudDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
