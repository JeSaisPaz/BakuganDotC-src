// bdc 0x0899b7c0 UiWorldMapPulseCursor
#include "bdc.h"

/* Per-frame glow of the area cursor sprite (data `+0x60`) of `UiWorldMap`: shared
   add-colour pulse `UiPulseStepTint` (40 frames, state `+0x434`). */

void UiWorldMapPulseCursor(UiScreen *screen)
{
  GfxSprite **sprites = (GfxSprite **)screen->data;

  UiPulseStepTint(40.0f, sprites[0x18], &((UiWorldMap *)screen)->cursorPulse);
}
