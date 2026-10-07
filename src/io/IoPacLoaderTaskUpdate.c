// bdc 0x089fe5a0 IoPacLoaderTaskUpdate
#include "bdc.h"

/* Update (vtable slot 2) of the `.pac` loader task (id 10110): steps the package loader
   (`IoPacLoaderUpdate`) when it exists. */

void IoPacLoaderTaskUpdate(void)

{
  if (IoHasPacLoader()) {
    IoPacLoaderUpdate(IoGetPacLoader());
  }
}
