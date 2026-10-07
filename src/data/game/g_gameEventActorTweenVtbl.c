// bdc 0x08af4344 g_gameEventActorTweenVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameEventActorTweenVtbl = {
    {0}, { .fn = (void *)GameEventActorTweenDtor }, { .fn = (void *)GameEventActionBeginNop },
    { .fn = (void *)GameEventActorTweenStop }, { .fn = (void *)GameEventActorTweenUpdate },
    { .fn = (void *)GameEventActorTweenGetKind }, { .fn = (void *)GameEventActorTweenGetTarget },
};
