// bdc 0x089e0c60 GfxModelChainDeleteAll
#include "bdc.h"

/* Deletes every object of a model chain starting at `first` (next at `+4`) through its virtual
   deleting destructor (vtable `+0x14`, slot 1, flags 3). */

void GfxModelChainDeleteAll(CoreObject *first)

{
  CoreObject *next;

  if (first != (CoreObject *)0x0) {
    next = first->next;
    while (1) {
      if (first != (CoreObject *)0x0) {
        const VtblEntry *e = &((const VtblEntry *)first->vtable)[1];
        ((void (*)(void *, s32))e->fn)((char *)first + e->delta, 3);
      }
      if (next == (CoreObject *)0x0) break;
      first = next;
      next = next->next;
    }
  }
  return;
}
