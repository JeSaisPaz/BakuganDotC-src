// bdc 0x089f9260 GfxFabClipDraw
#include "bdc.h"

/* Draws a `.fab` clip: collects its placed objects with their depth (`+0xe8`) into a 256-entry
   stack array, sorts them (`GfxCombSortByDepth`) and draws each with `GfxFabObjectDraw` (which recurses
   back here for nested clips). Returns the advanced display-list pointer. */

typedef struct FabDepthPair {
  GfxFabObject *obj;
  float depth;
} FabDepthPair;

u32 *GfxFabClipDraw(void *clip, u32 *dl, float *mtx, void *color, void *colorAdd)
{
  FabDepthPair pairs[256];
  u32 count = 0;
  GfxFabObject *obj;
  int i;

  for (obj = (GfxFabObject *)((GfxFabClip *)clip)->objHead; obj != 0;
       obj = (GfxFabObject *)obj->base.next) {
    pairs[count].depth = obj->depth;
    pairs[count].obj = obj;
    count++;
  }
  if ((int)count > 0) {
    if ((int)count > 1) {
      GfxCombSortByDepth(pairs, count);
    }
    for (i = 0; i < (int)count; i++) {
      dl = GfxFabObjectDraw(pairs[i].obj, dl, mtx, color, colorAdd);
    }
  }
  return dl;
}
