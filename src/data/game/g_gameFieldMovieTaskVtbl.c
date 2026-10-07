// bdc 0x08af178c g_gameFieldMovieTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameFieldMovieTaskVtbl = {
    {0}, { .fn = (void *)GameFieldMovieTaskDtor }, { .fn = (void *)GameFieldMovieTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
