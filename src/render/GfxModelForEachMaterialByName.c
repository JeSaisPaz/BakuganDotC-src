// bdc 0x089e0ac0 GfxModelForEachMaterialByName
#include "bdc.h"

/* Calls `fn(matState, arg)` for every material whose name contains `substr`. */

void GfxModelForEachMaterialByName(GfxModel *self, const char *substr, void *fn, void *arg)

{
  const char *matName;
  char *found;
  void *state;
  int index;
  int count;

  count = self->materialCount;
  for (index = 0; index < count; index++) {
    matName = GfxModelGetMaterialName(self, index);
    found = strstr(matName, substr);
    if (found != (char *)0x0) {
      state = GfxModelGetMaterialState(self, index);
      ((void (*)(void *, void *))fn)(state, arg);
    }
  }
}
