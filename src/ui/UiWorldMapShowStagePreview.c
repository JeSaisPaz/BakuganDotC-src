// bdc 0x0899d774 UiWorldMapShowStagePreview
#include "bdc.h"

/* Shows the preview picture of stage `stage` of the selected area in `UiWorldMap`
   (`UiWorldMapSetStageImage` on sprite 0x1a plus the frames 0x20/0x21) when the stage is cleared
   (below `+0x11c2[areaId]`); otherwise hides them. */

void UiWorldMapShowStagePreview(UiScreen *screen, u8 stage)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  s8 area = map->areaId;
  GfxSprite **sprites = (GfxSprite **)screen->data;
  GfxSprite *sprite = sprites[0x68 / 4];

  if (stage < map->clearedStage[map->areaGroup[area]]) {
    UiWorldMapSetStageImage(screen, sprite, (u8)area, stage);
    sprites[0x68 / 4]->flags |= 1;
    sprites[0x80 / 4]->flags |= 1;
    sprites[0x84 / 4]->flags |= 1;
  } else {
    sprite->flags &= ~1u;
    sprites[0x80 / 4]->flags &= ~1u;
    sprites[0x84 / 4]->flags &= ~1u;
  }
}
