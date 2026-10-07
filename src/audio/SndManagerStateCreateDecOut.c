// bdc 0x089c6fbc SndManagerStateCreateDecOut
#include "bdc.h"

/* State 4 of the sound manager (`SndManagerStep`): creates the first decoder output
   (`SndDecOutCreate` with both arguments 0, the object that the BGM/movie decoders write PCM
   into) and moves the manager to state 5 (running). */

void SndManagerStateCreateDecOut(SndManager *mgr)

{
  SndDecOutCreate(0,0);
  mgr->state = 5;
  return;
}

