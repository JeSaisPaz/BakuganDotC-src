// bdc 0x089fa0b4 IoDiscTryResume
#include "bdc.h"

/* When the `CODiscSimple` disc reader (`g_discSimple`) is suspended (state 9) and a resume was
   requested (`+0xed`), and the power manager reports running again (`CorePowerIsRunning`), moves
   to state 10 and clears the request. Returns 1 on resume. */

int IoDiscTryResume(IoDiscSimple *self)

{
  CorePower *power;
  int running;
  int resumed;
  
  resumed = 0;
  CoreLockAcquire(self->lock);
  if ((self->state == 9) && (self->resumeRequested != '\0')) {
    power = CorePowerGet();
    running = CorePowerIsRunning(power);
    if (running != 0) {
      self->state = 10;
      self->resumeRequested = '\0';
      resumed = 1;
    }
  }
  CoreLockRelease(self->lock);
  return resumed;
}

