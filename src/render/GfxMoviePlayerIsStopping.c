// bdc 0x089d6184 GfxMoviePlayerIsStopping
#include "bdc.h"

/* Returns the stopping flag (set once `scePsmfPlayerStop` succeeded in `GfxMoviePlayerStopStep`).
   The binary returns the raw byte (`lbu`); the flag only holds 0/1. */
bool GfxMoviePlayerIsStopping(GfxMoviePlayer *player)
{
    return player->stopping != 0;
}
