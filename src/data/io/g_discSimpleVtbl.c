// bdc 0x08af5894 g_discSimpleVtbl
#include "bdc.h"

__typeof__(VtblEntry[11]) g_discSimpleVtbl = {
    {0}, { .fn = (void *)IoDiscSimpleUpdate }, { .fn = (void *)IoDiscSimpleSlot2Nop },
    { .fn = (void *)IoDiscSimpleStateOpen }, { .fn = (void *)IoDiscSimpleStateGetSize },
    { .fn = (void *)IoDiscSimpleStateRead }, { .fn = (void *)IoDiscSimpleStateStreamRead },
    { .fn = (void *)IoDiscSimpleStateClose }, { .fn = (void *)IoDiscSimpleStateRewind },
    { .fn = (void *)IoDiscSimpleStateGetLba }, { .fn = (void *)IoDiscSimpleStateReopen },
};
