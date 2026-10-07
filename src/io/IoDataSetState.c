// bdc 0x089fb9d8 IoDataSetState
#include "bdc.h"

/* Sets the `IoDataExec` state (`+0x2c`) of a data request (`COData`) under its lock. */

void IoDataSetState(IoData *self, int state)

{
  CoreLockAcquire(self->lock);
  self->state = state;
  CoreLockRelease(self->lock);
  return;
}

