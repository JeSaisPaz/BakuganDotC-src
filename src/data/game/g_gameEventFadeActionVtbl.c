// bdc 0x08af42d4 g_gameEventFadeActionVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameEventFadeActionVtbl = {
    {0}, { .fn = (void *)GameEventFadeActionDtor }, { .fn = (void *)GameEventFadeActionBegin },
    { .fn = (void *)GameEventFadeActionStop }, { .fn = (void *)GameEventFadeActionUpdate },
    { .fn = (void *)GameEventFadeActionGetKind }, { .fn = (void *)GameEventFadeActionGetTarget },
};
