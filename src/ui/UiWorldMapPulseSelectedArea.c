// bdc 0x0899b7ec UiWorldMapPulseSelectedArea
#include "bdc.h"

/* Per-frame glow of the selected area button (data `[+0x109c]`) of `UiWorldMap`
   through the shared add-colour pulse `UiCursorGlowStep`. */

void UiWorldMapPulseSelectedArea(UiScreen *screen)
{
  GfxSprite **sprites = (GfxSprite **)screen->data;

  UiCursorGlowStep(sprites[((UiWorldMap *)screen)->areaId]);
}
