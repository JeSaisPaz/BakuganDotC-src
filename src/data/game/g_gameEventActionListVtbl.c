// bdc 0x08af41b4 g_gameEventActionListVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameEventActionListVtbl = {
    {0}, { .fn = (void *)GameEventActionListDtor }, { .fn = (void *)GameEventActionListUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
