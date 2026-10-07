// bdc 0x089c053c SndListenerIsMuted
#include "bdc.h"

/* Returns the listener's mute flag (SndListener+0x24, flag24). In SndEmitterUpdateAll a
   non-zero value makes every emitter stop its voice, silencing the positional sound effects without
   destroying the emitters. */
u8 SndListenerIsMuted(SndListener *listener)
{
    return listener->flag24;
}
