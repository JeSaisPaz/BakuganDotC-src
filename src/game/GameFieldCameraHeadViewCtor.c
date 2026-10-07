// bdc 0x088c902c GameFieldCameraHeadViewCtor
#include "bdc.h"

/* Constructor of the field camera's head view helper (`cam+0x3d0`, a `GameFieldCameraSpringCtor`
   holder plus target vectors at `+0x10`/`+0x20`): allocates the spring state
   (`GameFieldCameraSpringCtor`). */

void *GameFieldCameraHeadViewCtor(void *view)

{
  GameFieldCameraSpringCtor(view);
  return view;
}

