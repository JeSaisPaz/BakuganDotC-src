// bdc 0x089d57e4 GfxMovieHasPlayer
#include "bdc.h"

/* Returns whether `g_moviePlayer` is non-null (singleton accessor, named by `bdc singleton`). */
bool GfxMovieHasPlayer(void)
{
    return g_moviePlayer != NULL;
}
