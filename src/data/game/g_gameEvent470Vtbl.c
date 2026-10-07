// bdc 0x08af425c g_gameEvent470Vtbl
#include "bdc.h"

__typeof__(VtblEntry[15]) g_gameEvent470Vtbl = {
    {0}, { .fn = (void *)GameEvent470Dtor }, { .fn = (void *)GameEventUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)GameEventDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
    { .fn = (void *)GameEvent470Slot7 }, { .fn = (void *)GameEvent470UpdateActorTurn },
    { .fn = (void *)GameEvent470ShowNextIcon }, { .fn = (void *)GameEvent470SkipLabels },
    { .fn = (void *)GameEvent470ExecCommand }, { .fn = (void *)GameEvent470CheckWait },
    { .fn = (void *)GameEvent470ApplyAxisOffset }, { .fn = (void *)GameEvent470Nop },
};
