// bdc 0x0899bad4 UiWorldMapStartSelectFlash
#include "bdc.h"

/* Starts the 4-frame add-colour flash (`UiFlashStart`, flash slot 0) on the selected area button of
   `UiWorldMap`; `UiWorldMapSelectFlashDone` waits for it. */

void UiWorldMapStartSelectFlash(UiScreen *screen)
{
  GfxSprite **sprites = (GfxSprite **)screen->data;

  UiFlashStart(4.0f, sprites[((UiWorldMap *)screen)->areaId], 0, 0);
}
