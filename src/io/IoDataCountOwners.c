// bdc 0x089fbf50 IoDataCountOwners
#include "bdc.h"

/* Returns the number of owner references of a data request (`COData`). */

int IoDataCountOwners(IoData *self)

{
  int count;
  CoreNode *node;
  
  count = 0;
  for (node = self->owners; node != (CoreNode *)0x0; node = node->next) {
    count = count + 1;
  }
  return count;
}
