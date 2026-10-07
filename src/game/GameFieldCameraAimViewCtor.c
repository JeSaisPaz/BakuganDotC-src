// bdc 0x088cc73c GameFieldCameraAimViewCtor
#include "bdc.h"

/* Constructor of the field camera's throw aim helper (`cam+0x400`, a `GameFieldCameraSpringCtor`
   holder plus eye/look-at targets `+0x10`/`+0x20` and aim direction `+0x30`): allocates the spring
   state (`GameFieldCameraSpringCtor`). */

void *GameFieldCameraAimViewCtor(void *view)

{
  GameFieldCameraSpringCtor(view);
  return view;
}

