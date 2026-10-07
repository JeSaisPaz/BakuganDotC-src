// bdc 0x0895ae30 UiEquipResetCursor
#include "bdc.h"

/* Resets the cursor of the Bakugan/gear loadout screen before a battle (task 302, `UiEquipCtor`;
   per player picks Bakugan from a 20-face grid (`"baku_face_%02d"`) and equipment
   (`"c_setting_soubi_l_sol_%02d"`), shown on `"menu_daiza.gmo"` pedestals; `+0x4cda` = number of
   players (2 or 4 layouts), `+0x4cdb` = player being edited) onto the current selection: grid row
   (`onRandom` == 0) uses the cursor sprite `spriteIdx[6]` placed on grid cell
   `spriteIdx[1] + gridCursor` (icon via `UiEquipSetCursorTexture`) and the player icon
   `spriteIdx[7]` above it, otherwise the random-button cursor `spriteIdx[0xd]`
   (`UiEquipSetRandomCursorTexture`) and icon `spriteIdx[0xe]`; the unused pair is hidden.
   Resets the glow and pulse state (`UiCursorGlowReset`, `UiPulseReset`, `cursorPulse` = 0) and
   restores scale 1 and the stored depth of the grid, pulse and button sprites. */

void UiEquipResetCursor(UiEquip *self)
{
  GfxSprite **sprites;
  GfxSprite *cursor;
  GfxSprite *icon;
  GfxSprite *cell;
  int i;

  UiCursorGlowReset();
  sprites = (GfxSprite **)self->base.data;
  self->cursorPulse = 0.0f;
  cursor = sprites[self->spriteIdx[6]];
  if (self->onRandom == 0) {
    UiEquipSetCursorTexture(self, cursor, (u8)self->editPlayer);
    UiPulseReset((UiPulse *)&self->tweens[self->spriteIdx[6]]);
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[6]]->flags |= 1;
    sprites = (GfxSprite **)self->base.data;
    GfxSpriteCenterPivot(sprites[self->spriteIdx[6]]);
    sprites = (GfxSprite **)self->base.data;
    UiSpriteSetScaleRotation(sprites[self->spriteIdx[6]], 1.0f, 1.0f, 0.0f);
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[6]]->alpha = 1.0f;
    cursor = ((GfxSprite **)self->base.data)[self->spriteIdx[6]];
    cursor->addColor[0] = 0.3f;
    cursor->addColor[1] = 0.3f;
    cursor->addColor[2] = 0.3f;
    cursor->addColor[3] = 1.0f;
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[6]]->posX = sprites[self->spriteIdx[1] + self->gridCursor]->posX;
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[6]]->posY = sprites[self->spriteIdx[1] + self->gridCursor]->posY;
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[6]]->posZ = self->spriteZ[self->spriteIdx[6]];
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0xd]]->flags &= ~1u;
    sprites = (GfxSprite **)self->base.data;
    UiPulseInit(sprites[self->spriteIdx[6]], sprites[self->spriteCount],
                (UiPulse *)&self->tweens[self->spriteCount]);
  }
  else {
    cursor->flags &= ~1u;
    sprites = (GfxSprite **)self->base.data;
    UiEquipSetRandomCursorTexture(self, sprites[self->spriteIdx[0xd]], (u8)self->editPlayer);
    UiPulseReset((UiPulse *)&self->tweens[self->spriteIdx[0xd]]);
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0xd]]->flags |= 1;
    sprites = (GfxSprite **)self->base.data;
    GfxSpriteCenterPivot(sprites[self->spriteIdx[0xd]]);
    sprites = (GfxSprite **)self->base.data;
    UiSpriteSetScaleRotation(sprites[self->spriteIdx[0xd]], 1.0f, 1.0f, 0.0f);
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0xd]]->alpha = 1.0f;
    cursor = ((GfxSprite **)self->base.data)[self->spriteIdx[0xd]];
    cursor->addColor[0] = 0.3f;
    cursor->addColor[1] = 0.3f;
    cursor->addColor[2] = 0.3f;
    cursor->addColor[3] = 1.0f;
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0xd]]->posZ = self->spriteZ[self->spriteIdx[0xd]];
    sprites = (GfxSprite **)self->base.data;
    UiPulseInit(sprites[self->spriteIdx[0xd]], sprites[self->spriteCount],
                (UiPulse *)&self->tweens[self->spriteCount]);
  }

  icon = ((GfxSprite **)self->base.data)[self->spriteIdx[7]];
  if (self->onRandom == 0) {
    GfxSpriteSetCell(icon, 0.0f, (float)self->playerIconCell[self->editPlayer]);
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[7]]->flags |= 1;
    sprites = (GfxSprite **)self->base.data;
    GfxSpriteCenterPivot(sprites[self->spriteIdx[7]]);
    sprites = (GfxSprite **)self->base.data;
    UiSpriteSetScaleRotation(sprites[self->spriteIdx[7]], 1.0f, 1.0f, 0.0f);
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[7]]->alpha = 1.0f;
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[7]]->posX = sprites[self->spriteIdx[1] + self->gridCursor]->posX;
    sprites = (GfxSprite **)self->base.data;
    cell = sprites[self->spriteIdx[1] + self->gridCursor];
    icon = sprites[self->spriteIdx[7]];
    icon->posY = cell->posY - self->rowGapA * icon->scaleY;
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[7]]->posZ = self->spriteZ[self->spriteIdx[7]];
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0xe]]->flags &= ~1u;
  }
  else {
    icon->flags &= ~1u;
    sprites = (GfxSprite **)self->base.data;
    GfxSpriteSetCell(sprites[self->spriteIdx[0xe]], 0.0f,
                     (float)self->playerIconCell[self->editPlayer]);
    UiPulseReset((UiPulse *)&self->tweens[self->spriteIdx[0xe]]);
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0xe]]->flags |= 1;
    sprites = (GfxSprite **)self->base.data;
    GfxSpriteCenterPivot(sprites[self->spriteIdx[0xe]]);
    sprites = (GfxSprite **)self->base.data;
    UiSpriteSetScaleRotation(sprites[self->spriteIdx[0xe]], 1.0f, 1.0f, 0.0f);
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0xe]]->alpha = 1.0f;
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0xe]]->posZ = self->spriteZ[self->spriteIdx[0xe]];
  }

  /* grid sprite ranges (20 each), then the spriteIdx[0x4b] range of spriteIdx[0x4c] sprites */
  for (i = self->spriteIdx[1]; i < self->spriteIdx[1] + 20; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = self->spriteIdx[2]; i < self->spriteIdx[2] + 20; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = self->spriteIdx[3]; i < self->spriteIdx[3] + 20; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = self->spriteIdx[0x4b]; i < self->spriteIdx[0x4b] + self->spriteIdx[0x4c]; i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }

  sprites = (GfxSprite **)self->base.data;
  UiSpriteSetScaleRotation(sprites[self->spriteIdx[4]], 1.0f, 1.0f, 0.0f);
  sprites = (GfxSprite **)self->base.data;
  sprites[self->spriteIdx[4]]->posZ = self->spriteZ[self->spriteIdx[4]];
  sprites = (GfxSprite **)self->base.data;
  UiSpriteSetScaleRotation(sprites[self->spriteIdx[5]], 1.0f, 1.0f, 0.0f);
  sprites = (GfxSprite **)self->base.data;
  sprites[self->spriteIdx[5]]->posZ = self->spriteZ[self->spriteIdx[5]] - 1.0f;
}
