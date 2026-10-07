// bdc 0x08af2dbc g_gameStoryMovieVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameStoryMovieVtbl = {
    {0}, { .fn = (void *)GameStoryMovieDtor }, { .fn = (void *)GameStoryMovieUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
