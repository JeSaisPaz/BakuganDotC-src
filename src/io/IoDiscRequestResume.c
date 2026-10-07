// bdc 0x089fa134 IoDiscRequestResume
#include "bdc.h"

/* Sets the resume-request flag `+0xed` of the `CODiscSimple` disc reader (`g_discSimple`) under
   its lock; `IoDiscTryResume` acts on it. */

void IoDiscRequestResume(IoDiscSimple *self)

{
  CoreLockAcquire(self->lock);
  self->resumeRequested = '\x01';
  CoreLockRelease(self->lock);
  return;
}

