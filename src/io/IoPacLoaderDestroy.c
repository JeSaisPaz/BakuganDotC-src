// bdc 0x089fd7dc IoPacLoaderDestroy
#include "bdc.h"

/* Destroys the `.pac` package loader singleton `g_ioPacLoader` (`IoPacLoaderDelete``(loader,
   3)`) and clears it. Counterpart of `IoPacLoaderCreate`; called from `IoPacLoaderTaskDtor`. */

void IoPacLoaderDestroy(void)

{
  if (g_ioPacLoader != (void *)0x0) {
    IoPacLoaderDelete(g_ioPacLoader,3);
    g_ioPacLoader = (void *)0x0;
  }
  return;
}

