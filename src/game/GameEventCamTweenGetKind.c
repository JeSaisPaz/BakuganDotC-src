// bdc 0x08a2c754 GameEventCamTweenGetKind
#include "bdc.h"

/* Entry 5 (kind, `+0x2c`) of the event camera tween vtable `0x08af430c`: returns kind 1. */

s32 GameEventCamTweenGetKind(GameEventCamTween *self)

{
  return 1;
}

