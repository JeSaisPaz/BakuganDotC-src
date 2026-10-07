// bdc 0x0899e0f4 UiWorldMapZoomSelectedStage
#include "bdc.h"

/* Per-frame zoom-in of the selected stage row of `UiWorldMap`'s stage list: raises
   `zoomT` by 0.1 while it is below 1 (scale = 1 + zoomT × 0.1, capped at 1.1) and applies the scale
   to the plate (sprite `0x32 + stage`), every row element (0x3a, 0x3d, 0x40, 0x43, 0x46, 0x37, the
   five score digits 0x49.., 0x58), sprite 0x35 and the slot's name glyphs (at 0.8 × scale),
   re-positioning each relative to the plate by its cached offset × scale and bringing the row
   forward (Z −200..−204). Also clears flag 0x20 of the row's sprite 0x46. */

/* Scale `sprite`, set its Z and place it at the plate position minus `offset` × its own scale. */
static void UiWorldMapZoomRowSprite(GfxSprite **sprites, int idx, int stage, float scale, float z,
                                    const float *offset)
{
  GfxSprite *plate;
  GfxSprite *sprite;

  UiSpriteSetScaleRotation(sprites[idx], scale, scale, 0.0f);
  sprites[idx]->posZ = z;
  plate = sprites[0x32 + stage];
  sprite = sprites[idx];
  sprite->posX = plate->posX - offset[0] * sprite->scaleX;
  plate = sprites[0x32 + stage];
  sprite = sprites[idx];
  sprite->posY = plate->posY - offset[1] * sprite->scaleY;
}

void UiWorldMapZoomSelectedStage(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite **sprites = (GfxSprite **)screen->data;
  int stage = map->stage;
  float zoom = map->zoomT;
  float scale;
  float glyphScale;
  GfxSprite *glyph;
  int i;

  if (zoom < 1.0f) {
    zoom = zoom + 0.1f;
    map->zoomT = zoom;
  }
  scale = zoom * 0.100000024f + 1.0f;
  if (!(scale <= 1.1f)) {
    scale = 1.1f;
  }

  UiSpriteSetScaleRotation(sprites[0x32 + stage], scale, scale, 0.0f);
  sprites[0x32 + stage]->posZ = -200.0f;

  UiWorldMapZoomRowSprite(sprites, 0x3a + stage, stage, scale, -202.0f, map->panelOffset[2]);
  UiWorldMapZoomRowSprite(sprites, 0x3d + stage, stage, scale, -201.0f, map->panelOffset[1]);
  UiWorldMapZoomRowSprite(sprites, 0x40 + stage, stage, scale, -202.0f, map->panelOffset[3]);
  UiWorldMapZoomRowSprite(sprites, 0x43 + stage, stage, scale, -201.0f, map->panelOffset[4]);
  UiWorldMapZoomRowSprite(sprites, 0x46 + stage, stage, scale, -202.0f, map->panelOffset[5]);
  sprites[0x46 + stage]->flags &= ~0x20u;
  UiWorldMapZoomRowSprite(sprites, 0x37 + stage, stage, scale, -201.0f, map->panelOffset[6]);
  for (i = 0; i < 5; i++) {
    UiWorldMapZoomRowSprite(sprites, 0x49 + stage * 5 + i, stage, scale, -201.0f,
                            map->scoreOffset[stage * 5 + i]);
  }
  UiWorldMapZoomRowSprite(sprites, 0x58 + stage, stage, scale, -201.0f, map->panelOffset[8]);

  UiSpriteSetScaleRotation(sprites[0x35], scale, scale, 0.0f);
  sprites[0x35]->posZ = -204.0f;

  glyph = map->textSlot[stage].glyphs;
  if (0.0f < map->textSlot[stage].glyphCount) {
    glyphScale = scale * 0.8f;
    i = 0;
    do {
      UiSpriteSetScaleRotation(glyph, glyphScale, glyphScale, 0.0f);
      glyph->posZ = -202.0f;
      glyph->posX = sprites[0x32 + stage]->posX - map->glyphOffset[stage][i][0] * scale;
      glyph->posY = sprites[0x32 + stage]->posY - map->glyphOffset[stage][i][1] * scale;
      glyph = glyph->next;
      i++;
    } while ((float)i < map->textSlot[stage].glyphCount);
  }
}
