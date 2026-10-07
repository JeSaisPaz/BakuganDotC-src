// bdc 0x0898ef64 UiCollectionFigureGetDetailScale
#include "bdc.h"

/* Returns the display scale of figure `id` in the detail view (`view` 0) or the second detail view
   (`view` 1) of `UiCollectionFigure`, from the 21×2 float table
   `0x08ac3af8` (1.4–3.2). */

float UiCollectionFigureGetDetailScale(UiCollectionFigure *self, u8 view, u8 id)

{
  float scales [42];
  
  memcpy(scales,g_collectionFigureDetailScale,0xa8);
  return scales[id * 2 + view];
}

