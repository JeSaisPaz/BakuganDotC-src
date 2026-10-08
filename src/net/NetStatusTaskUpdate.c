// bdc 0x08943d78 NetStatusTaskUpdate
#include "bdc.h"

/* Update of the netplay status overlay (id 2002): calls the state method selected by `state` (0..4)
   through the `MemberFnPtr` table `g_netStatusStateTable` (`0x08a9cffc`); for a virtual entry
   (`index` != 0) `pfn` is the vtable-pointer offset inside the adjusted object. */

void NetStatusTaskUpdate(NetStatusTask *self)

{
  const MemberFnPtr *entry;
  char *obj;
  void (*fn)(void *);

  if ((-1 < self->state) && (self->state < 5)) {
    entry = &g_netStatusStateTable[self->state];
    obj = (char *)self + entry->delta;
    fn = (void (*)(void *))entry->pfn;
    if (entry->index != 0) {
      const MemberFnPtr *v = *(const MemberFnPtr **)(obj + (intptr_t)entry->pfn) + entry->index;
      fn = (void (*)(void *))v->pfn;
      obj = obj + v->delta;
    }
    fn(obj);
  }
  return;
}
