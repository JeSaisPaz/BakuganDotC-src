// bdc 0x089e0a40 GfxModelForEachMaterial
#include "bdc.h"

/* Calls `fn(matState, arg)` for the state record of every material of the model
   (`GfxModelGetMaterialState`). */

void GfxModelForEachMaterial(GfxModel *self, void *fn, void *arg)

{
  int index;
  int count;

  count = self->materialCount;
  index = 0;
  if (0 < count) {
    do {
      void *state = GfxModelGetMaterialState(self, index);
      ((void (*)(void *, void *))fn)(state, arg);
      index = index + 1;
    } while (index < count);
  }
  return;
}
