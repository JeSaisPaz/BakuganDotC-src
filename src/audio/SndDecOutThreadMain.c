// bdc 0x089bc5d8 SndDecOutThreadMain
#include "bdc.h"

/* Entry point of the three decoder threads `MyThread-Sound-Sub0/1/2` (slots 6..8 of
   `g_threadTable`, priority 17, stack 0x8000): reads the channel number from the 4-byte argument
   block and loops `SndDecOutThreadStep(SndDecOutGet(channel))` until it returns 0, then calls
   `SndDecOutDestroy``(channel)` and returns 0. */

s32 SndDecOutThreadMain(SceSize argSize, void *argp)

{
  SndDecOut *dec;
  s32 running;
  s32 channel;
  
  if (argp != (void *)0x0) {
    channel = *(s32 *)argp;
    do {
      dec = SndDecOutGet(channel);
      running = SndDecOutThreadStep(dec);
    } while (running != 0);
    SndDecOutDestroy(channel);
  }
  return 0;
}

