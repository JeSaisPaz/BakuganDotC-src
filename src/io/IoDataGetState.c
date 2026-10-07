// bdc 0x089fba18 IoDataGetState
#include "bdc.h"

/* Returns the `IoDataExec` state (`+0x2c`) of a data request (`COData`) under its lock. */

int IoDataGetState(IoData *self)

{
  int state;
  
  CoreLockAcquire(self->lock);
  state = self->state;
  CoreLockRelease(self->lock);
  return state;
}

