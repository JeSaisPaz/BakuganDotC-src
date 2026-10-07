// bdc 0x088cc764 GameFieldCameraAimViewDtor
#include "bdc.h"

/* Destructor of the field camera's throw aim helper (`cam+0x400`, a `GameFieldCameraSpringCtor`
   holder plus eye/look-at targets `+0x10`/`+0x20` and aim direction `+0x30`): frees the spring
   state; frees the object when `flags & 1`. */

void GameFieldCameraAimViewDtor(void *view, u32 flags)

{
  if ((view != (void *)0x0) && (GameFieldCameraSpringDtor(view,0), (flags & 1) != 0)) {
    MemLock();
    MemFree(view,(char *)0x0,0);
    MemUnlock();
  }
  return;
}

