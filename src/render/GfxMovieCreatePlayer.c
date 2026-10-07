// bdc 0x089d55d0 GfxMovieCreatePlayer
#include "bdc.h"

/* Creates the movie singletons after the modules are up: sets `g_gfxDisplay``->frameSkip` to 1,
   allocates (low heap) and zeroes the 4-byte block `g_movieBlock` if it is missing, and, if
   missing, allocates (low heap) the 0x58-byte movie player `g_moviePlayer` and runs
   `GfxMoviePlayerInit` on it (NULL stays stored if the allocation fails). */

void GfxMovieCreatePlayer(void)
{
  bool fromLow;
  void *block;
  GfxMoviePlayer *player;

  g_gfxDisplay->frameSkip = 1;
  if (g_movieBlock == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    block = MemAlloc(4, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    g_movieBlock = block;
    memset(block, 0, 4);
  }
  if (g_moviePlayer == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    player = MemAlloc(sizeof(GfxMoviePlayer), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (player != NULL) {
      GfxMoviePlayerInit(player);
    }
    g_moviePlayer = player;
  }
}
