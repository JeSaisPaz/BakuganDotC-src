// bdc 0x088853ac BtlShadowCtorForActor
#include "bdc.h"

/* Constructs a field actor's shadow object (owner `actor`, mode 1: billboard) and initialises it
   (`BtlShadowInit`). Called by `ActorCtor` (`actor+0x16c`). */

BtlShadow *BtlShadowCtorForActor(BtlShadow *shadow, void *actor)
{
    shadow->owner = actor;
    shadow->mode = 1;
    BtlShadowInit(shadow);
    return shadow;
}
