// bdc 0x089fe3dc IoDecodeMngUpdate
#include "bdc.h"

/* One step of the decode thread (`BootDecodeThread`): walks the manager's job list (`head`,
   following `next`, read before the job is touched), deleting cancelled jobs through their
   virtual destructor (vtable slot 1: flags 2 = destruct only when `MemPoolFree` gave the slot
   back to the manager's `pool`, else flags 3 = destruct and free the heap block) and running
   `IoDecodeStep` on jobs that do not wait for input; then sleeps 200 us when a step ran, or puts
   the thread to sleep (`BootSleepCurrentThread`) when nothing did. */

typedef void (*IoDecodeJobDtorFn)(void *self, u32 flags);

void IoDecodeMngUpdate(IoDecodeMng *self)
{
  bool worked;
  IoDecodeJob *job;
  IoDecodeJob *next;
  const VtblEntry *dtor;

  job = (IoDecodeJob *)self->base.head;
  worked = false;
  while (job != NULL) {
    next = (IoDecodeJob *)job->base.next;
    if (IoDecodeIsCancelled(job)) {
      if (self->pool != NULL && MemPoolFree(self->pool, job)) {
        dtor = &((const VtblEntry *)job->base.vtable)[1];
        ((IoDecodeJobDtorFn)dtor->fn)((u8 *)job + dtor->delta, 2);
        job = NULL;
      }
      if (job != NULL) {
        dtor = &((const VtblEntry *)job->base.vtable)[1];
        ((IoDecodeJobDtorFn)dtor->fn)((u8 *)job + dtor->delta, 3);
      }
    } else if (!IoDecodeIsWaitingInput(job)) {
      worked = true;
      IoDecodeStep(job);
    }
    job = next;
  }
  if (worked) {
    sceKernelDelayThreadCB(200);
  } else {
    BootSleepCurrentThread();
  }
}
