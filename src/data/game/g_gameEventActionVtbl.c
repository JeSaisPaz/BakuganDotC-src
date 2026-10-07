// bdc 0x08af6e20 g_gameEventActionVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameEventActionVtbl = {
    {0}, { .fn = (void *)GameEventActionDtor }, { .fn = (void *)GameEventActionBeginNop },
    { .fn = (void *)GameEventActionStopNop }, { .fn = (void *)CxxPureVirtualCalled },
    { .fn = (void *)CxxPureVirtualCalled }, { .fn = (void *)GameEventActionGetTarget },
};
