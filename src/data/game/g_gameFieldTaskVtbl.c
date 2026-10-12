// bdc 0x08af2cfc g_gameFieldTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameFieldTaskVtbl = {
    {0}, { .fn = (void *)GameFieldDtor }, { .fn = (void *)GameFieldUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)GameFieldDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)GameFieldGetField },
};
