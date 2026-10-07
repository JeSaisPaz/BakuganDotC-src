// bdc 0x0899d854 UiWorldMapRefreshStageCursor
#include "bdc.h"

/* Redraws the stage-list selection of `UiWorldMap`: resets the pulses
   (`UiCursorGlowReset`, `stagePulse`, `zoomT = 0`), shows the stage cursor sprite 0x35 centred,
   unscaled, opaque and darkened (add colour 0.3) on plate `0x32 + stage` and starts the
   highlight clone (sprite 0x5d, `randomPulse`, `UiPulseInit`), restores scale/Z of the plates
   0x32..0x34 and re-attaches every row element (sprites 0x37..0x48, 0x58..0x5a with the offsets
   cached by `UiWorldMapCachePanelOffsets`, digits 0x49..0x57 with
   `UiWorldMapCacheScoreOffsets`, the name glyphs of the text slots below the area's
   cleared-stage count with the offsets of `UiWorldMapLayoutStageNames`, scale 0.8, Z -40) to
   its plate, then shows the stage preview (`UiWorldMapShowStagePreview`). */

void UiWorldMapRefreshStageCursor(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite *cursor;
  GfxSprite *glyph;
  int i;
  int slot;
  int n;

  UiCursorGlowReset();
  map->zoomT = 0.0f;
  UiPulseReset(&map->stagePulse);
  ((GfxSprite **)screen->data)[0x35]->flags |= 1;
  GfxSpriteCenterPivot(((GfxSprite **)screen->data)[0x35]);
  UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[0x35], 1.0f, 1.0f, 0.0f);
  ((GfxSprite **)screen->data)[0x35]->alpha = 1.0f;
  cursor = ((GfxSprite **)screen->data)[0x35];
  cursor->addColor[3] = 1.0f;
  cursor->addColor[0] = 0.3f;
  cursor->addColor[1] = 0.3f;
  cursor->addColor[2] = 0.3f;
  ((GfxSprite **)screen->data)[0x35]->posX =
      ((GfxSprite **)screen->data)[0x32 + map->stage]->posX;
  ((GfxSprite **)screen->data)[0x35]->posY =
      ((GfxSprite **)screen->data)[0x32 + map->stage]->posY;
  ((GfxSprite **)screen->data)[0x35]->posZ = map->spriteBaseZ[0x35];
  UiPulseInit(((GfxSprite **)screen->data)[0x35], ((GfxSprite **)screen->data)[0x5d],
              &map->randomPulse);

  /* plates 0x32..0x34 */
  for (i = 0x32; i < 0x35; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)screen->data)[i]->posZ = map->spriteBaseZ[i];
  }

  /* panel elements, each group 0..2 follows plate 0x32 + (i - first) */
  for (i = 0x3a; i < 0x3d; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)screen->data)[i]->posZ = map->spriteBaseZ[i];
    ((GfxSprite **)screen->data)[i]->posX =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x3a)]->posX - map->panelOffset[2][0];
    ((GfxSprite **)screen->data)[i]->posY =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x3a)]->posY - map->panelOffset[2][1];
  }
  for (i = 0x3d; i < 0x40; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)screen->data)[i]->posZ = map->spriteBaseZ[i];
    ((GfxSprite **)screen->data)[i]->posX =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x3d)]->posX - map->panelOffset[1][0];
    ((GfxSprite **)screen->data)[i]->posY =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x3d)]->posY - map->panelOffset[1][1];
  }
  for (i = 0x40; i < 0x43; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)screen->data)[i]->posZ = map->spriteBaseZ[i];
    ((GfxSprite **)screen->data)[i]->posX =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x40)]->posX - map->panelOffset[3][0];
    ((GfxSprite **)screen->data)[i]->posY =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x40)]->posY - map->panelOffset[3][1];
  }
  for (i = 0x43; i < 0x46; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)screen->data)[i]->posZ = map->spriteBaseZ[i];
    ((GfxSprite **)screen->data)[i]->posX =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x43)]->posX - map->panelOffset[4][0];
    ((GfxSprite **)screen->data)[i]->posY =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x43)]->posY - map->panelOffset[4][1];
  }
  for (i = 0x46; i < 0x49; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)screen->data)[i]->posZ = map->spriteBaseZ[i];
    ((GfxSprite **)screen->data)[i]->posX =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x46)]->posX - map->panelOffset[5][0];
    ((GfxSprite **)screen->data)[i]->posY =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x46)]->posY - map->panelOffset[5][1];
  }
  for (i = 0x37; i < 0x3a; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)screen->data)[i]->posZ = map->spriteBaseZ[i];
    ((GfxSprite **)screen->data)[i]->posX =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x37)]->posX - map->panelOffset[6][0];
    ((GfxSprite **)screen->data)[i]->posY =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x37)]->posY - map->panelOffset[6][1];
  }

  /* score digits 0x49..0x57, five per plate */
  for (i = 0x49; i < 0x58; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)screen->data)[i]->posZ = map->spriteBaseZ[i];
    ((GfxSprite **)screen->data)[i]->posX =
        ((GfxSprite **)screen->data)[0x32 + (u8)((i - 0x49) / 5)]->posX -
        map->scoreOffset[i - 0x49][0];
    ((GfxSprite **)screen->data)[i]->posY =
        ((GfxSprite **)screen->data)[0x32 + (u8)((i - 0x49) / 5)]->posY -
        map->scoreOffset[i - 0x49][1];
  }

  for (i = 0x58; i < 0x5b; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)screen->data)[i]->posZ = map->spriteBaseZ[i];
    ((GfxSprite **)screen->data)[i]->posX =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x58)]->posX - map->panelOffset[8][0];
    ((GfxSprite **)screen->data)[i]->posY =
        ((GfxSprite **)screen->data)[0x32 + (u8)(i - 0x58)]->posY - map->panelOffset[8][1];
  }

  /* stage-name glyphs of the cleared stages' text slots */
  for (slot = 0; slot < 3; slot++) {
    if (slot >= map->clearedStage[map->areaGroup[map->areaId]])
      break;
    glyph = map->textSlot[slot].glyphs;
    n = 0;
    if (0.0f < map->textSlot[slot].glyphCount) {
      do {
        UiSpriteSetScaleRotation(glyph, 0.8f, 0.8f, 0.0f);
        glyph->posZ = -40.0f;
        glyph->posX = ((GfxSprite **)screen->data)[0x32 + slot]->posX -
                      map->glyphOffset[slot][n][0];
        glyph->posY = ((GfxSprite **)screen->data)[0x32 + slot]->posY -
                      map->glyphOffset[slot][n][1];
        glyph = glyph->next;
        n++;
      } while ((float)n < map->textSlot[slot].glyphCount);
    }
  }
  UiWorldMapShowStagePreview(screen, (u8)map->stage);
}
