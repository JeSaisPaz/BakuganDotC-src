// bdc 0x08998f9c UiWorldMapRefreshAreaCursor
#include "bdc.h"

/* Redraws the area selection of `UiWorldMap`: resets the cursor glow and
   `cursorPulse`, clears `zoomT`, shows the cursor sprite 0x18 centred at scale 1 (alpha 1, add colour
   0.3/0.3/0.3/1, Z `spriteBaseZ[0x18]`) on area button `areaId` and starts its highlight clone 0x5d
   (`UiPulseInit`, `randomPulse`); restores scale and base Z of sprites 0..0x17 and 0x2a..0x31, lights
   the selected area button (`UiWorldMapSetAreaButton`) and dims the others. If the area's bit in
   `unlockMask` is set, it shows the stage picture 0x1a (`UiWorldMapSetStageImage`, stage `stage`),
   the flag 0x1b (`UiWorldMapSetFlag`; visible only if `areaHasFlag[areaId]`) and sprites
   0x20/0x21; otherwise it hides those four preview sprites. */

#define SPRITES ((GfxSprite **)screen->data)

void UiWorldMapRefreshAreaCursor(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite *sprite;
  int i;

  UiCursorGlowReset();
  map->zoomT = 0.0f;
  UiPulseReset(&map->cursorPulse);
  sprite = SPRITES[0x18];
  sprite->flags |= 1;
  GfxSpriteCenterPivot(SPRITES[0x18]);
  UiSpriteSetScaleRotation(SPRITES[0x18], 1.0f, 1.0f, 0.0f);
  SPRITES[0x18]->alpha = 1.0f;
  sprite = SPRITES[0x18];
  sprite->addColor[3] = 1.0f;
  sprite->addColor[0] = 0.3f;
  sprite->addColor[1] = 0.3f;
  sprite->addColor[2] = 0.3f;
  SPRITES[0x18]->posZ = map->spriteBaseZ[0x18];
  SPRITES[0x18]->posX = SPRITES[map->areaId]->posX;
  SPRITES[0x18]->posY = SPRITES[map->areaId]->posY;
  UiPulseInit(SPRITES[0x18], SPRITES[0x5d], &map->randomPulse);

  /* Area buttons 0..7: the selected one lit, the others plain. */
  for (i = 0; i < 8; i++) {
    UiSpriteSetScaleRotation(SPRITES[i], 1.0f, 1.0f, 0.0f);
    SPRITES[i]->posZ = map->spriteBaseZ[i];
    sprite = SPRITES[i];
    if (i == map->areaId) {
      sprite->addColor[0] = 0.3f;
      sprite->addColor[1] = 0.3f;
      sprite->addColor[2] = 0.3f;
      sprite->addColor[3] = 1.0f;
      UiWorldMapSetAreaButton(screen, SPRITES[i], true);
    } else {
      sprite->addColor[0] = 0.0f;
      sprite->addColor[1] = 0.0f;
      sprite->addColor[2] = 0.0f;
      sprite->addColor[3] = 1.0f;
      UiWorldMapSetAreaButton(screen, SPRITES[i], false);
    }
  }
  for (i = 8; i < 0x10; i++) {
    UiSpriteSetScaleRotation(SPRITES[i], 1.0f, 1.0f, 0.0f);
    SPRITES[i]->posZ = map->spriteBaseZ[i];
  }
  for (i = 0x10; i < 0x18; i++) {
    UiSpriteSetScaleRotation(SPRITES[i], 1.0f, 1.0f, 0.0f);
    SPRITES[i]->posZ = map->spriteBaseZ[i];
  }
  for (i = 0x2a; i < 0x32; i++) {
    UiSpriteSetScaleRotation(SPRITES[i], 1.0f, 1.0f, 0.0f);
    SPRITES[i]->posZ = map->spriteBaseZ[i];
  }

  /* Preview: flag 0x1b, stage picture 0x1a, sprites 0x20/0x21. */
  sprite = SPRITES[0x1b];
  if ((map->unlockMask & (1 << map->areaId)) == 0) {
    sprite->flags &= ~1u;
    SPRITES[0x1a]->flags &= ~1u;
    SPRITES[0x20]->flags &= ~1u;
    SPRITES[0x21]->flags &= ~1u;
  } else {
    if (map->areaHasFlag[map->areaId] == 0) {
      sprite->flags &= ~1u;
    } else {
      sprite->flags |= 1;
    }
    SPRITES[0x1a]->flags |= 1;
    UiWorldMapSetFlag(screen, SPRITES[0x1b], (u8)map->areaId);
    UiWorldMapSetStageImage(screen, SPRITES[0x1a], (u8)map->areaId, (u8)map->stage);
    SPRITES[0x20]->flags |= 1;
    SPRITES[0x21]->flags |= 1;
  }
}
