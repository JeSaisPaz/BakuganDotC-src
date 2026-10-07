// bdc 0x0899d738 UiWorldMapStartStageFlash
#include "bdc.h"

/* Starts the 4-frame add-colour flash (`UiFlashStart`, slot 0) on the selected stage plate (sprite
   `0x32 + +0x109e`) of `UiWorldMap`. */

void UiWorldMapStartStageFlash(UiScreen *screen)
{
  GfxSprite **sprites = (GfxSprite **)screen->data;

  UiFlashStart(4.0f, sprites[0x32 + ((UiWorldMap *)screen)->stage], 0, 0);
}
