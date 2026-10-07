// bdc 0x08af41ec g_gameEventVtbl
#include "bdc.h"

__typeof__(VtblEntry[14]) g_gameEventVtbl = {
    {0}, { .fn = (void *)GameEventDtor }, { .fn = (void *)GameEventUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)GameEventDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
    { .fn = (void *)GameEventSlot7 }, { .fn = (void *)GameEventEnd },
    { .fn = (void *)GameEventShowNextIconNop }, { .fn = (void *)GameEventSkipLabelsNop },
    { .fn = (void *)GameEventExecCommandNop }, { .fn = (void *)GameEventCheckWaitReturnTrue },
    { .fn = (void *)GameEventApplyAxisOffsetNop },
};
