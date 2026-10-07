// bdc 0x089e0c60 GfxModelChainDeleteAll
#include "bdc.h"

/* Deletes every object of a model chain starting at `first` (next at `+4`) through its virtual
   deleting destructor (vtable `+0x14` slot `+0xc`, flags 3). */

typedef struct GfxVtable {
  unsigned char head[8];
  short adjust;
  short pad;
  void (*fn)(void *self, int flags);
} GfxVtable;

void GfxModelChainDeleteAll(CoreObject *first)

{
  CoreObject *next;

  if (first != (CoreObject *)0x0) {
    next = first->next;
    while (1) {
      if (first != (CoreObject *)0x0) {
        const GfxVtable *e = (const GfxVtable *)first->vtable;
        e->fn((char *)first + e->adjust, 3);
      }
      if (next == (CoreObject *)0x0) break;
      first = next;
      next = next->next;
    }
  }
  return;
}
