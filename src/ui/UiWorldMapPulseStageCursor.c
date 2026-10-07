// bdc 0x0899d5a8 UiWorldMapPulseStageCursor
#include "bdc.h"

/* Per-frame glow of the stage-list cursor sprite (data `+0xd4`) of `UiWorldMap`:
   shared add-colour pulse `UiPulseStepTint` (40 frames, state `+0x8bc`). */

void UiWorldMapPulseStageCursor(UiScreen *screen)
{
  GfxSprite **sprites = (GfxSprite **)screen->data;

  UiPulseStepTint(40.0f, sprites[0x35], &((UiWorldMap *)screen)->stagePulse);
}
