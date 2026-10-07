// bdc 0x088ea7b4 GameEventPropPlayMotion
#include "bdc.h"

/* Would start motion `slot` on the prop's model (`ActorBallPlayMotion`, speed `speed`), but the
   model-kind range test (`kind < 0x59 && kind > 0x6a`) can never pass, so it does nothing. */

void GameEventPropPlayMotion(void *prop, s16 slot, u8 loop, u8 speed)

{
  CoreObject *ball;
  
  ball = *(CoreObject **)prop;
  if (((ball != (CoreObject *)0x0) && (ball->unk08 < 0x59)) && (0x6a < ball->unk08)) {
    ActorBallPlayMotion((float)speed,ball,(int)slot,loop,false);
  }
  return;
}

