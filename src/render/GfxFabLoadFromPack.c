// bdc 0x089f8678 GfxFabLoadFromPack
#include "bdc.h"

/* Finds file `name` in the loaded pack chain (`0x08ac520c`, `CorePackChainFindNamed`, entry
   stored at `fab+0x1c`) and initialises the fab object from it (`GfxFabInit`). */

void GfxFabLoadFromPack(GfxFab *fab, char *name)

{
  void *data;

  data = CorePackChainFindNamed((CorePack *)g_ioLzsPackages, name, &fab->name);
  GfxFabInit(fab, data);
}
