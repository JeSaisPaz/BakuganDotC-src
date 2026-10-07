// bdc 0x089f89cc GfxFabGetBitmap
#include "bdc.h"

/* Returns bitmap texture `index` of the fab (`+0x78[index]`), or NULL when out of range (`+0x76`).
    */

void *GfxFabGetBitmap(GfxFab *fab, int index)

{
  if (index < (int)fab->bitmapCount) {
    return fab->bitmaps[index];
  }
  return NULL;
}
