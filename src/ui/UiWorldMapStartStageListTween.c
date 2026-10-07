// bdc 0x089a1020 UiWorldMapStartStageListTween
#include "bdc.h"

/* Builds and slides in/out the rank-mode stage list of `UiWorldMap`
   (`spriteTween[0x32..]`, `UiTweenBeginSlide` flags 0xb). Coming in (`out` = 0) lays out up to
   three rows for the selected area (`stageCount[group]` stages, of which `clearedStage[group]`
   cleared, group = `areaGroup[areaId]`): plates 0x32..0x34 (`UiWorldMapSetStagePlate`, alt look
   for cleared rows), the area number 0x36 (`UiWorldMapSetDigitCell`), row sprites 0x37..0x39,
   rank letters 0x3a..0x3c (`UiWorldMapSetRankLetter`, `stageRecords[group * 4 + row].rank`),
   character portraits 0x46..0x48 (`UiWorldMapSetCharaFace`, `.id`), scores
   (`UiWorldMapSetStageScore`, `.score`), the "empty" sprites 0x58..0x5a (shown for uncleared rows,
   only row 0 unless the area has three stages) and names (`UiWorldMapLayoutStageNames`). The
   sprites are placed at `panelPos - panelOffset[k]` (one `panelGapY` lower unless the area has
   three stages), each sliding down from the first row by `row * panelGapY`; going out reverses the
   slides. */

void UiWorldMapStartStageListTween(UiScreen *screen, u8 out)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite **sprites;
  GfxSprite *sprite;
  float *off;
  u8 group;
  int i;
  int k;
  int row;

  if (out == 0) {
    /* stage plates */
    for (i = 0x32; i < 0x35; i++) {
      row = i - 0x32;
      sprites = (GfxSprite **)screen->data;
      group = map->areaGroup[map->areaId];
      sprite = sprites[i];
      if (row < map->stageCount[group]) {
        sprite->flags |= 1;
      } else {
        sprite->flags &= ~1u;
      }
      group = map->areaGroup[map->areaId];
      UiWorldMapSetStagePlate(screen, sprites[i], row < map->clearedStage[group]);
      sprites = (GfxSprite **)screen->data;
      group = map->areaGroup[map->areaId];
      sprite = sprites[i];
      if (map->stageCount[group] == 3) {
        sprite->posY = map->panelPos[1];
      } else {
        sprite->posY = map->panelPos[1] + map->panelGapY;
      }
      UiTweenBeginSlide(1.0f, 0.0f,
                        (map->panelPos[1] + map->panelGapY * (float)row) - map->panelPos[1], out,
                        sprites[i], &map->spriteTween[i], 0xb);
    }
    /* area number */
    for (i = 0x36; i < 0x37; i++) {
      sprites = (GfxSprite **)screen->data;
      group = map->areaGroup[map->areaId];
      sprite = sprites[i];
      sprite->posX = map->panelPos[0] - map->panelOffset[0][0];
      if (map->stageCount[group] == 3) {
        sprites[i]->posY = map->panelPos[1] - map->panelOffset[0][1];
      } else {
        sprites[i]->posY = (map->panelPos[1] - map->panelOffset[0][1]) + map->panelGapY;
      }
      sprites[i]->flags |= 1;
      UiWorldMapSetDigitCell(screen, sprites[i], map->areaGroup[map->areaId]);
      sprites = (GfxSprite **)screen->data;
      UiTweenBeginSlide(1.0f, 0.0f, 0.0f, out, sprites[i], &map->spriteTween[i], 0xb);
    }
    /* rank letters (0x3a..0x3c) and their row decorations */
    for (i = 0x3a; i < 0x43; i++) {
      k = i - 0x3a;
      row = k % 3;
      sprites = (GfxSprite **)screen->data;
      group = map->areaGroup[map->areaId];
      sprite = sprites[i];
      if (row < map->clearedStage[group]) {
        sprite->flags |= 1;
        if (i >= 0x3a && i < 0x3d) {
          group = map->areaGroup[map->areaId];
          UiWorldMapSetRankLetter(screen, sprites[i], map->stageRecords[group * 4 + row].rank);
        }
      } else {
        sprite->flags &= ~1u;
      }
      sprites = (GfxSprite **)screen->data;
      group = map->areaGroup[map->areaId];
      sprite = sprites[i];
      switch (k) {
      case 0:
      case 1:
      case 2:
        off = map->panelOffset[2];
        break;
      case 3:
      case 4:
      case 5:
        off = map->panelOffset[1];
        break;
      default: /* 6..8 */
        off = map->panelOffset[3];
        break;
      }
      sprite->posX = map->panelPos[0] - off[0];
      if (map->stageCount[group] == 3) {
        sprites[i]->posY = map->panelPos[1] - off[1];
      } else {
        sprites[i]->posY = (map->panelPos[1] - off[1]) + map->panelGapY;
      }
      sprite = sprites[i];
      UiTweenBeginSlide(1.0f, 0.0f, (sprite->posY + map->panelGapY * (float)row) - sprite->posY,
                        out, sprite, &map->spriteTween[i], 0xb);
    }
    /* character portraits (0x46..0x48) and their row decorations */
    for (i = 0x43; i < 0x49; i++) {
      k = i - 0x43;
      row = k % 3;
      sprites = (GfxSprite **)screen->data;
      group = map->areaGroup[map->areaId];
      sprite = sprites[i];
      if (row < map->clearedStage[group]) {
        sprite->flags |= 1;
        if (i >= 0x46 && i < 0x49) {
          group = map->areaGroup[map->areaId];
          UiWorldMapSetCharaFace(screen, sprites[i], map->stageRecords[group * 4 + row].id);
        }
      } else {
        sprite->flags &= ~1u;
      }
      sprites = (GfxSprite **)screen->data;
      group = map->areaGroup[map->areaId];
      sprite = sprites[i];
      if (k == 3 || k == 4 || k == 5) {
        off = map->panelOffset[5];
      } else {
        off = map->panelOffset[4];
      }
      sprite->posX = map->panelPos[0] - off[0];
      if (map->stageCount[group] == 3) {
        sprites[i]->posY = map->panelPos[1] - off[1];
      } else {
        sprites[i]->posY = (map->panelPos[1] - off[1]) + map->panelGapY;
      }
      sprite = sprites[i];
      UiTweenBeginSlide(1.0f, 0.0f, (sprite->posY + map->panelGapY * (float)row) - sprite->posY,
                        out, sprite, &map->spriteTween[i], 0xb);
    }
    /* row sprites 0x37..0x39 */
    for (i = 0x37; i < 0x3a; i++) {
      row = (i - 0x37) % 3;
      sprites = (GfxSprite **)screen->data;
      group = map->areaGroup[map->areaId];
      sprite = sprites[i];
      if (row < map->clearedStage[group]) {
        sprite->flags |= 1;
      } else {
        sprite->flags &= ~1u;
      }
      sprites = (GfxSprite **)screen->data;
      group = map->areaGroup[map->areaId];
      sprite = sprites[i];
      sprite->posX = map->panelPos[0] - map->panelOffset[6][0];
      if (map->stageCount[group] == 3) {
        sprites[i]->posY = map->panelPos[1] - map->panelOffset[6][1];
      } else {
        sprites[i]->posY = (map->panelPos[1] - map->panelOffset[6][1]) + map->panelGapY;
      }
      sprite = sprites[i];
      UiTweenBeginSlide(1.0f, 0.0f, (sprite->posY + map->panelGapY * (float)row) - sprite->posY,
                        out, sprite, &map->spriteTween[i], 0xb);
    }
    /* scores of the cleared rows */
    for (row = 0; row < 3; row++) {
      group = map->areaGroup[map->areaId];
      if (row < map->clearedStage[group]) {
        if (map->stageCount[group] == 3) {
          UiWorldMapSetStageScore(0.0f, screen, (u8)row, map->stageRecords[group * 4 + row].score);
        } else {
          UiWorldMapSetStageScore(map->panelGapY, screen, (u8)row,
                                  map->stageRecords[group * 4 + row].score);
        }
      }
    }
    /* score digits 0x49..0x57, five per row */
    for (i = 0x49; i < 0x58; i++) {
      sprite = ((GfxSprite **)screen->data)[i];
      UiTweenBeginSlide(1.0f, 0.0f,
                        (sprite->posY + map->panelGapY * (float)((i - 0x49) / 5)) - sprite->posY,
                        out, sprite, &map->spriteTween[i], 0xb);
    }
    /* "empty" sprites 0x58..0x5a: shown for uncleared rows (only row 0 unless three stages) */
    for (i = 0x58; i < 0x5b; i++) {
      row = (i - 0x58) % 3;
      sprites = (GfxSprite **)screen->data;
      group = map->areaGroup[map->areaId];
      sprite = sprites[i];
      if (row < map->clearedStage[group]) {
        sprite->flags &= ~1u;
      } else if (map->stageCount[group] == 3) {
        sprite->flags |= 1;
      } else if (row == 0) {
        sprite->flags |= 1;
      } else {
        sprite->flags &= ~1u;
      }
      sprites = (GfxSprite **)screen->data;
      group = map->areaGroup[map->areaId];
      sprite = sprites[i];
      sprite->posX = map->panelPos[0] - map->panelOffset[8][0];
      if (map->stageCount[group] == 3) {
        sprites[i]->posY = map->panelPos[1] - map->panelOffset[8][1];
      } else {
        sprites[i]->posY = (map->panelPos[1] - map->panelOffset[8][1]) + map->panelGapY;
      }
      sprite = sprites[i];
      UiTweenBeginSlide(1.0f, 0.0f, (sprite->posY + map->panelGapY * (float)row) - sprite->posY,
                        out, sprite, &map->spriteTween[i], 0xb);
    }
    UiWorldMapLayoutStageNames(screen);
  } else {
    for (i = 0x32; i < 0x35; i++) {
      UiTweenBeginSlide(1.0f, 0.0f,
                        map->panelPos[1] - (map->panelPos[1] + map->panelGapY * (float)(i - 0x32)),
                        out, ((GfxSprite **)screen->data)[i], &map->spriteTween[i], 0xb);
    }
    for (i = 0x36; i < 0x37; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, 0.0f, out, ((GfxSprite **)screen->data)[i],
                        &map->spriteTween[i], 0xb);
    }
    for (i = 0x3a; i < 0x43; i++) {
      sprite = ((GfxSprite **)screen->data)[i];
      UiTweenBeginSlide(1.0f, 0.0f,
                        sprite->posY - (sprite->posY + map->panelGapY * (float)((i - 0x3a) % 3)),
                        out, sprite, &map->spriteTween[i], 0xb);
    }
    for (i = 0x43; i < 0x49; i++) {
      sprite = ((GfxSprite **)screen->data)[i];
      UiTweenBeginSlide(1.0f, 0.0f,
                        sprite->posY - (sprite->posY + map->panelGapY * (float)((i - 0x43) % 3)),
                        out, sprite, &map->spriteTween[i], 0xb);
    }
    for (i = 0x37; i < 0x3a; i++) {
      sprite = ((GfxSprite **)screen->data)[i];
      UiTweenBeginSlide(1.0f, 0.0f,
                        sprite->posY - (sprite->posY + map->panelGapY * (float)((i - 0x37) % 3)),
                        out, sprite, &map->spriteTween[i], 0xb);
    }
    for (i = 0x49; i < 0x58; i++) {
      sprite = ((GfxSprite **)screen->data)[i];
      UiTweenBeginSlide(1.0f, 0.0f,
                        sprite->posY - (sprite->posY + map->panelGapY * (float)((i - 0x49) / 5)),
                        out, sprite, &map->spriteTween[i], 0xb);
    }
    for (i = 0x58; i < 0x5b; i++) {
      sprite = ((GfxSprite **)screen->data)[i];
      UiTweenBeginSlide(1.0f, 0.0f,
                        sprite->posY - (sprite->posY + map->panelGapY * (float)((i - 0x58) % 3)),
                        out, sprite, &map->spriteTween[i], 0xb);
    }
  }
}
