// bdc 0x089fe28c IoDecodeMngNewJob
#include "bdc.h"

/* Creates a decode job for the decode manager: takes a slot from its pool (`pool`, +0x30) or,
   when there is no pool or it is full, heap-allocates 0x10b0 bytes (low heap end when bit0 of
   `dest` is set) and constructs it with `IoDecodeCtor`. Returns the job (the caller links it),
   or NULL when the heap allocation failed. */

IoDecodeJob *IoDecodeMngNewJob(IoDecodeMng *self, void *src, void *arg, u8 *dest, bool streamed)
{
  bool fromLow = ((uintptr_t)dest & 1) != 0;
  bool prevFromLow;
  IoDecodeJob *job = NULL;

  if (self->pool != NULL) {
    job = MemPoolAlloc(self->pool);
  }
  if (job != NULL) {
    IoDecodeCtor(job, src, arg, dest, streamed);
    return job;
  }
  MemLock();
  prevFromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(fromLow);
  job = MemAlloc(sizeof(IoDecodeJob), NULL, 0);
  MemSetAllocFromLow(prevFromLow);
  MemUnlock();
  if (job == NULL) {
    return NULL;
  }
  IoDecodeCtor(job, src, arg, dest, streamed);
  return job;
}
