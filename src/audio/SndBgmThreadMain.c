// bdc 0x089bc628 SndBgmThreadMain
#include "bdc.h"

/* Entry point of the BGM player threads `MyThread-Sound-Bgm0/1/2` (slots 9..11 of
   `g_threadTable`, priority 33, stack 0x8000): reads the channel from the 4-byte argument block
   and loops `SndBgmPlayerThreadStep(SndBgmPlayerGet(channel))` until it returns 0, then calls
   `SndBgmPlayerDestroy``(channel)` and returns 0. */

s32 SndBgmThreadMain(SceSize argSize, void *argp)

{
  SndBgmPlayer *player;
  s32 running;
  s32 index;

  if (argp != (void *)0x0) {
    index = *(s32 *)argp;
    do {
      player = SndBgmPlayerGet(index);
      running = SndBgmPlayerThreadStep(player);
    } while (running != 0);
    SndBgmPlayerDestroy(index);
  }
  return 0;
}
