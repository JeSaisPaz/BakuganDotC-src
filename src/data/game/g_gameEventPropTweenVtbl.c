// bdc 0x08af437c g_gameEventPropTweenVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameEventPropTweenVtbl = {
    {0}, { .fn = (void *)GameEventPropTweenDtor }, { .fn = (void *)GameEventActionBeginNop },
    { .fn = (void *)GameEventPropTweenStop }, { .fn = (void *)GameEventPropTweenUpdate },
    { .fn = (void *)GameEventPropTweenGetKind }, { .fn = (void *)GameEventPropTweenGetTarget },
};
