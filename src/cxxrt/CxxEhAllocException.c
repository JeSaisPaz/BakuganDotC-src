// bdc 0x08a03dd0 CxxEhAllocException
#include "bdc.h"

/* Allocates the storage for a thrown object of `size` bytes (`CxxEhAlloc`) and pushes its
   exception record for type `type` (destructor from `type+0xc`). Returns the object storage; the
   caller constructs the object and calls `CxxThrow`. */

void *CxxEhAllocException(void *type, u32 size, u8 flags)

{
  void *object;
  void *dtor;

  dtor = ((CxxTypeInfo *)type)->dtor;
  object = CxxEhAlloc(size);
  CxxEhPushException(type,dtor,flags,0,0,'\0',object,0,(void *)0x0);
  return object;
}
