// bdc 0x089fd844 IoPacLoaderCtor
#include "bdc.h"

/* Constructor of the `.pac` package loader: clears `state` (0 = idle) and `entry`. */

void *IoPacLoaderCtor(void *loader)

{
  IoPacLoader *self = (IoPacLoader *)loader;

  self->state = 0;
  self->entry = (void *)0;
  return loader;
}
