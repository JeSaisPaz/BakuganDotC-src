// bdc 0x089fb704 IoDataCtor
#include "bdc.h"

/* Constructor of a data request (`COData`), a 0x60-byte `CoreNode` with vtable `0x08af58ec`: path
   fields `+0x24` (owned copy) and `+0x28` (path in use, default `0x08aa3e58` = empty), state
   `+0x2c` 0, flags `+0x30` 0, done/owned bytes `+0x38`/`+0x39`, buffer `+0x3c` 0, `+0x40` = 1, user
   word `+0x44`, owner-reference list `+0x50`, `+0x5c` = -1. Returns `data`. */

IoData *IoDataCtor(IoData *self)

{
  CoreNodeCtor(&self->base,(CoreNode *)0x0);
  (self->base).vtable = g_ioDataVtbl;
  self->path = "";
  self->state = 0;
  self->flags = 0;
  self->statusFlags = 0;
  self->done = '\0';
  self->ownsBuffer = '\0';
  self->buffer = (void *)0x0;
  self->streamBuffer = (void *)(uintptr_t)1;  /* sentinel: no stream buffer yet */
  self->bufferSize = 0;
  self->streamBufferSize = 0;
  self->lock = (CoreLock *)0x0;
  self->owners = (CoreNode *)0x0;
  self->decodeJob = (void *)0x0;
  self->allocator = (void *)0x0;
  self->loadedSize = -1;
  self->pathCopy = (char *)0x0;
  return self;
}

