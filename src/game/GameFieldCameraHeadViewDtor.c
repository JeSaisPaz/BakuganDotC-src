// bdc 0x088c9054 GameFieldCameraHeadViewDtor
#include "bdc.h"

/* Destructor of the field camera's head view helper (`cam+0x3d0`, a `GameFieldCameraSpringCtor`
   holder plus target vectors at `+0x10`/`+0x20`): frees the spring state; frees the object when
   `flags & 1`. */

void GameFieldCameraHeadViewDtor(void *view, u32 flags)

{
  if ((view != (void *)0x0) && (GameFieldCameraSpringDtor(view,0), (flags & 1) != 0)) {
    MemLock();
    MemFree(view,(char *)0x0,0);
    MemUnlock();
  }
  return;
}

