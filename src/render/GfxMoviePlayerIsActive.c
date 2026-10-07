// bdc 0x089d6120 GfxMoviePlayerIsActive
#include "bdc.h"

/* Returns the playing byte `active` of the PSMF movie player (non-zero from the moment
   `GfxMovieOpenThread` starts a movie until it is stopped). */

u8 GfxMoviePlayerIsActive(GfxMoviePlayer *player)
{
    return player->active;
}
