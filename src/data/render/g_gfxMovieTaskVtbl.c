// bdc 0x08af533c g_gfxMovieTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gfxMovieTaskVtbl = {
    {0}, { .fn = (void *)GfxMovieTaskDtor }, { .fn = (void *)GfxMovieTaskUpdate },
    { .fn = (void *)GfxMovieTaskSlot3 }, { .fn = (void *)GfxMovieTaskDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
