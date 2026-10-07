// bdc 0x089f85f4 GfxFabInit
#include "bdc.h"

/* Initialises a `.fab` animation object for file data `data` (`+0x18`): clears the
   definition/clip/label/bitmap fields, sets the transform (`+0x30`) to identity, the draw depth
   `+0x98` to 100.0, the loop flag `+0x9c` to 0 and the colour `+0xa0` to the default white vector
   (`0x08b00190`), then parses the file (`GfxFabLoad`). */

void GfxFabInit(GfxFab *fab, void *data)
{
  int i;
  int j;

  fab->data = data;
  fab->defs = 0;
  fab->defCount = 0;
  fab->unk08c = 0;
  fab->clips = 0;
  fab->unk090 = 0;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      fab->transform[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
  fab->labelCount = 0;
  fab->labels = 0;
  fab->bitmapCount = 0;
  fab->bitmaps = 0;
  fab->rootClip = 0;
  fab->depth = 100.0f;
  fab->loop = 0;
  fab->color[0] = g_colorWhite.x;
  fab->color[1] = g_colorWhite.y;
  fab->color[2] = g_colorWhite.z;
  fab->color[3] = g_colorWhite.w;
  GfxFabLoad(fab);
}
