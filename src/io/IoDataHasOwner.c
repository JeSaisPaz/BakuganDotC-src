// bdc 0x089fbf20 IoDataHasOwner
#include "bdc.h"

/* Returns 1 when `owner` is in the owner-reference list (`+0x50`) of a data request (`COData`). */

int IoDataHasOwner(IoData *self, void *owner)

{
  CoreNode *node;
  
  for (node = self->owners; node != (CoreNode *)0x0; node = node->next) {
    if (((IoDataOwnerRef *)node)->owner == owner) {
      return 1;
    }
  }
  return 0;
}
