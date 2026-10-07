// bdc 0x0899a658 UiWorldMapJetHoverStep
#include "bdc.h"

/* Hover animation of Marucho's jet on `UiWorldMap`: advances the two hover timers
   `jetHoverTimerY` / `jetHoverTimerX` by 1/120, sets the jet's `pos[1]` to
   `(1 − cos(pi * tY)) * 0.5 * 5 − 20` (−20..−15) and `pos[0]` to `(1 − cos(pi * tX)) * 0.5 * 8 − 4`
   (−4..+4), then copies `pos` into the translation row of the model's GMO root matrix and sets
   its w to 1.
   The cosines are `vcos` of the angle times the bank constant S703 (2/pi). */

void UiWorldMapJetHoverStep(UiScreen *screen)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  GmoModel *gmo;
  float t;
  float c;

  t = map->jetHoverTimerY + 0.008333334f;
  map->jetHoverTimerY = t;
  c = __builtin_cosf(t * 3.1415927f);
  map->jetModel->pos[1] = (1.0f - c) * 0.5f * 5.0f + -20.0f;

  t = map->jetHoverTimerX + 0.008333334f;
  map->jetHoverTimerX = t;
  c = __builtin_cosf(t * 3.1415927f);
  map->jetModel->pos[0] = (1.0f - c) * 0.5f * 8.0f + -4.0f;

  gmo = map->jetModel->data;
  gmo->rootMatrix[12] = map->jetModel->pos[0];
  gmo->rootMatrix[13] = map->jetModel->pos[1];
  gmo->rootMatrix[14] = map->jetModel->pos[2];
  gmo->rootMatrix[15] = map->jetModel->pos[3];
  map->jetModel->data->rootMatrix[15] = 1.0f;
}
