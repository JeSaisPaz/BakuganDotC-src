// bdc 0x089f9cf8 IoDiscSetBusy
#include "bdc.h"

/* Sets (`busy != 0`, timestamping it like a request) or clears the busy flag (`+0xb`) of the
   `CODiscSimple` disc reader (`g_discSimple`) under its lock. Returns 1 when the flag changed. */

int IoDiscSetBusy(IoDiscSimple *self, bool busy)

{
  int changed;
  
  changed = 0;
  CoreLockAcquire(self->lock);
  if (busy) {
    if (self->busy == '\0') {
      self->busy = '\x01';
      sceRtcGetCurrentClockLocalTime(&g_ioDiscClock.requestTime);
      g_ioDiscClock.lastActiveTime.year = g_ioDiscClock.requestTime.year;
      g_ioDiscClock.lastActiveTime.month = g_ioDiscClock.requestTime.month;
      g_ioDiscClock.lastActiveTime.day = g_ioDiscClock.requestTime.day;
      g_ioDiscClock.lastActiveTime.hour = g_ioDiscClock.requestTime.hour;
      g_ioDiscClock.lastActiveTime.minute = g_ioDiscClock.requestTime.minute;
      g_ioDiscClock.lastActiveTime.second = g_ioDiscClock.requestTime.second;
      g_ioDiscClock.lastActiveTime.microsecond = g_ioDiscClock.requestTime.microsecond;
      changed = 1;
    }
  }
  else if (self->busy != '\0') {
    self->busy = '\0';
    changed = 1;
  }
  CoreLockRelease(self->lock);
  return changed;
}

