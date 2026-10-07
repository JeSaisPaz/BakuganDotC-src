// bdc 0x089fdfc8 IoDecodeStep
#include "bdc.h"

/* Runs one decode step of a decode job (`CODecode`, 0x10b0 bytes) under its lock: a whole-buffer
   job decodes with `CoreLzssStreamDecode` (budget `+0x4c`) until it reports completion; a
   streamed job decodes the current input chunk (`+0x1064`, `+0x1070` bytes) and either finishes
   (dcache writeback of the output, done) or waits for the next chunk (`+0x27`). */

void IoDecodeStep(IoDecodeJob *self)

{
  int res;
  u32 inBytes;
  
  CoreLockAcquire(self->lock);
  if (self->done == '\0') {
    if (self->streamed == '\0') {
      res = CoreLzssStreamDecode(&self->stream,(self->stream).outSize);
      if (res == 0) {
        self->done = '\x01';
      }
    }
    else if (self->waitingInput == '\0') {
      inBytes = self->chunkSize;
      (self->stream).src = self->chunk;
      res = CoreLzssStreamDecode(&self->stream,inBytes);
      if (res == 0) {
        sceKernelDcacheWritebackInvalidateRange(self->output,self->outputSize);
        self->done = '\x01';
      }
      else {
        self->waitingInput = '\x01';
      }
    }
  }
  CoreLockRelease(self->lock);
  return;
}

