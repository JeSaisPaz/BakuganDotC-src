// bdc 0x089fbd38 IoDataAddOwner
#include "bdc.h"

/* Adds `owner` to the owner-reference list (`+0x50`) of a data request (`COData`) unless already
   present (`IoDataHasOwner`): a 0x28-byte `IoDataOwnerRef` (vtable `g_ioDataOwnerNodeVtbl`,
   owner at `+0x24`) from the pool `g_ioDataRefPool`, or from the heap (placement `fromLow`)
   when the pool is missing or full. The node becomes the list head when the list is empty, else
   it is linked to the head with `CoreNodeLink`. */

void IoDataAddOwner(IoData *self, void *owner, bool fromLow)
{
  bool prevFromLow;
  IoDataOwnerRef *ref;
  IoDataOwnerRef *mem;
  CoreNode *head;

  if (IoDataHasOwner(self, owner) != 0) {
    return;
  }
  ref = NULL;
  if (g_ioDataRefPool != NULL) {
    ref = MemPoolAlloc(g_ioDataRefPool);
  }
  if (ref == NULL) {
    MemLock();
    prevFromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(fromLow);
    mem = MemAlloc(sizeof(IoDataOwnerRef), NULL, 0);
    MemSetAllocFromLow(prevFromLow);
    MemUnlock();
    ref = NULL;
    if (mem != NULL) {
      CoreNodeCtor(&mem->base, NULL);
      mem->base.vtable = g_ioDataOwnerNodeVtbl;
      mem->owner = owner;
      ref = mem;
    }
  } else {
    CoreNodeCtor(&ref->base, NULL);
    ref->base.vtable = g_ioDataOwnerNodeVtbl;
    ref->owner = owner;
  }
  head = self->owners;
  if (head == NULL) {
    self->owners = &ref->base;
  } else {
    CoreNodeLink(&ref->base, head, 0);
  }
}
