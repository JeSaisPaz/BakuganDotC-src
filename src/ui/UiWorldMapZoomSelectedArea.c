// bdc 0x0899b818 UiWorldMapZoomSelectedArea
#include "bdc.h"

/* Per-frame zoom-in of the selected area of `UiWorldMap`: raises `zoomT` by 0.1
   while below 1 and scales (1 + 0.2·zoomT, at most 1.2) the area button (sprite `area`), sprite
   `area + 8` and the cursor sprite 0x18, and brings them and the button's two labels
   (`area + 0x10`, `area + 0x2a`) forward (Z −500..−503). */

void UiWorldMapZoomSelectedArea(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  float t;
  float scale;

  t = map->zoomT;
  if (t < 1.0f) {
    t = t + 0.1f;
    map->zoomT = t;
  }
  scale = t * 0.20000005f + 1.0f;
  if (!(scale <= 1.2f)) {
    scale = 1.2f;
  }
  UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[map->areaId], scale, scale, 0.0f);
  ((GfxSprite **)screen->data)[map->areaId]->posZ = -500.0f;
  UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[map->areaId + 8], scale, scale, 0.0f);
  ((GfxSprite **)screen->data)[map->areaId + 8]->posZ = -501.0f;
  UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[0x18], scale, scale, 0.0f);
  ((GfxSprite **)screen->data)[0x18]->posZ = -502.0f;
  ((GfxSprite **)screen->data)[map->areaId + 0x10]->posZ = -503.0f;
  ((GfxSprite **)screen->data)[map->areaId + 0x2a]->posZ = -503.0f;
}
