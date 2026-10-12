// bdc 0x08af430c g_gameEventCamTweenVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameEventCamTweenVtbl = {
    {0}, { .fn = (void *)GameEventCamTweenDtor }, { .fn = (void *)GameEventCamTweenBegin },
    { .fn = (void *)GameEventCamTweenStop }, { .fn = (void *)GameEventCamTweenUpdate },
    { .fn = (void *)GameEventCamTweenGetKind }, { .fn = (void *)GameEventActionGetTarget },
};
