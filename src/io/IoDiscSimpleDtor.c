// bdc 0x089f9648 IoDiscSimpleDtor
#include "bdc.h"

/* Destructor of the `CODiscSimple` disc reader (`g_discSimple`): restores vtable `0x08af5894`
   (`+0x100`), deletes its two read-buffer nodes (`+0x38`, `+0x3c`, virtual dtor), destroys the
   `"Mutex_CODiscSimple"` lock (`+0x44`) and frees the object when `flags & 1`. */

void IoDiscSimpleDtor(IoDiscSimple *self, u32 flags)

{
  IoDiscBufNode *node;
  const VtblEntry *entry;
  int i;
  
  if (self != (IoDiscSimple *)0x0) {
    self->vtbl = g_discSimpleVtbl;
    for (i = 0; i < 2; i++) {
      node = self->bufNodes[i];
      if (node != (IoDiscBufNode *)0x0) {
        entry = &((const VtblEntry *)node->base.vtable)[1];
        ((void (*)(void *, int))entry->fn)((u8 *)node + entry->delta, 3);
        self->bufNodes[i] = (IoDiscBufNode *)0x0;
      }
    }
    if (self->lock != (CoreLock *)0x0) {
      CoreLockDestroy(self->lock,3);
      self->lock = (CoreLock *)0x0;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}
