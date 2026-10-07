// bdc 0x08964ab8 UiEquipResetGearCursor
#include "bdc.h"

/* Resets the equipment-panel cursor of player `player` on the Bakugan/gear loadout screen before a
   battle (task 302, `UiEquipCtor`; per player picks Bakugan from a 20-face grid
   (`"baku_face_%02d"`) and equipment (`"c_setting_soubi_l_sol_%02d"`), shown on `"menu_daiza.gmo"`
   pedestals). Resets the glow (`UiCursorGlowReset`) and `cursorPulse`, then by the active row
   (`activeRow`, or with profile flag 0 set the local player's `gearRowFlags[localPlayer]`):
   row 0 (equipment list) shows the gear cursor `spriteIdx[0x2f] + player` with its icon
   (`UiEquipSetGearIconTexture`) on entry `panelEntry` of the row sprites `spriteIdx[0x25]`, hides
   the OK-button cursor `spriteIdx[0x23] + player` and draws the OK button inactive; row 1 (OK
   button) hides the gear cursor, shows the OK-button cursor in front (Z -200) and draws the OK
   button active; any other row touches neither. The shown cursor gets scale 1, alpha 1, a 0.3
   colour add and a fresh pulse (`UiPulseInit`). Finally restores scale 1 and the stored depth
   (`spriteZ`) of this player's sprites of nine panel groups, re-aligning the gear row sprites
   `spriteIdx[0x3f]` and `spriteIdx[0x4d]`. */

void UiEquipResetGearCursor(UiEquip *self, u8 player)
{
  GfxSprite **sprites;
  GfxSprite *cursor;
  s8 row;
  int idx;
  int i;

  UiCursorGlowReset();
  row = self->activeRow;
  self->cursorPulse = 0.0f;
  if (SaveGetProfileFlag0() != 0) {
    row = (s8)self->gearRowFlags[*(s32 *)self->localPlayer];
  }

  if (row <= 0) {
    if (row >= 0) {
      /* row 0: equipment list */
      sprites = (GfxSprite **)self->base.data;
      UiEquipSetGearIconTexture(self, sprites[self->spriteIdx[0x2f] + player], player);
      UiPulseReset((UiPulse *)&self->tweens[self->spriteIdx[0x2f] + player]);
      sprites = (GfxSprite **)self->base.data;
      sprites[self->spriteIdx[0x2f] + player]->layerMask = 0x10;
      sprites = (GfxSprite **)self->base.data;
      sprites[self->spriteIdx[0x2f] + player]->flags |= 1;
      sprites = (GfxSprite **)self->base.data;
      GfxSpriteCenterPivot(sprites[self->spriteIdx[0x2f] + player]);
      sprites = (GfxSprite **)self->base.data;
      UiSpriteSetScaleRotation(sprites[self->spriteIdx[0x2f] + player], 1.0f, 1.0f, 0.0f);
      sprites = (GfxSprite **)self->base.data;
      sprites[self->spriteIdx[0x2f] + player]->alpha = 1.0f;
      cursor = ((GfxSprite **)self->base.data)[self->spriteIdx[0x2f] + player];
      cursor->addColor[3] = 1.0f;
      cursor->addColor[0] = 0.3f;
      cursor->addColor[1] = 0.3f;
      cursor->addColor[2] = 0.3f;
      idx = self->spriteIdx[0x2f] + player;
      sprites = (GfxSprite **)self->base.data;
      sprites[idx]->posZ = self->spriteZ[idx];
      sprites = (GfxSprite **)self->base.data;
      sprites[self->spriteIdx[0x2f] + player]->posX =
          sprites[self->spriteIdx[0x25] + self->spriteIdx[0x26] * player + self->panelEntry]->posX;
      sprites = (GfxSprite **)self->base.data;
      sprites[self->spriteIdx[0x2f] + player]->posY =
          sprites[self->spriteIdx[0x25] + self->spriteIdx[0x26] * player + self->panelEntry]->posY;
      sprites = (GfxSprite **)self->base.data;
      UiPulseInit(sprites[self->spriteIdx[0x2f] + player], sprites[self->spriteCount],
                  (UiPulse *)&self->tweens[self->spriteCount]);
      sprites = (GfxSprite **)self->base.data;
      sprites[self->spriteIdx[0x23] + player]->flags &= ~1u;
      sprites = (GfxSprite **)self->base.data;
      UiEquipSetOkButtonTexture(self, sprites[self->spriteIdx[0x1f] + player], false);
      sprites = (GfxSprite **)self->base.data;
      UiSpriteSetHighlight(sprites[self->spriteIdx[0x1f] + player], 0);
    }
  }
  else if (row < 2) {
    /* row 1: OK button */
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0x2f] + player]->flags &= ~1u;
    UiPulseReset((UiPulse *)&self->tweens[self->spriteIdx[0x23] + player]);
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0x23] + player]->layerMask = 0x10;
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0x23] + player]->flags |= 1;
    sprites = (GfxSprite **)self->base.data;
    GfxSpriteCenterPivot(sprites[self->spriteIdx[0x23] + player]);
    sprites = (GfxSprite **)self->base.data;
    UiSpriteSetScaleRotation(sprites[self->spriteIdx[0x23] + player], 1.0f, 1.0f, 0.0f);
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0x23] + player]->alpha = 1.0f;
    cursor = ((GfxSprite **)self->base.data)[self->spriteIdx[0x23] + player];
    cursor->addColor[3] = 1.0f;
    cursor->addColor[0] = 0.3f;
    cursor->addColor[1] = 0.3f;
    cursor->addColor[2] = 0.3f;
    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[0x23] + player]->posZ = -200.0f;
    sprites = (GfxSprite **)self->base.data;
    UiPulseInit(sprites[self->spriteIdx[0x23] + player], sprites[self->spriteCount],
                (UiPulse *)&self->tweens[self->spriteCount]);
    sprites = (GfxSprite **)self->base.data;
    UiEquipSetOkButtonTexture(self, sprites[self->spriteIdx[0x1f] + player], true);
    sprites = (GfxSprite **)self->base.data;
    UiSpriteSetHighlight(sprites[self->spriteIdx[0x1f] + player], 1);
  }

  for (i = self->spriteIdx[0x1f] + self->spriteIdx[0x20] * player;
       i < self->spriteIdx[0x1f] + self->spriteIdx[0x20] * (player + 1); i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = self->spriteIdx[0x21] + self->spriteIdx[0x22] * player;
       i < self->spriteIdx[0x21] + self->spriteIdx[0x22] * (player + 1); i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = self->spriteIdx[0x25] + self->spriteIdx[0x26] * player;
       i < self->spriteIdx[0x25] + self->spriteIdx[0x26] * (player + 1); i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = self->spriteIdx[0x27] + self->spriteIdx[0x28] * player;
       i < self->spriteIdx[0x27] + self->spriteIdx[0x28] * (player + 1); i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = self->spriteIdx[0x2d] + self->spriteIdx[0x2e] * player;
       i < self->spriteIdx[0x2d] + self->spriteIdx[0x2e] * (player + 1); i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = self->spriteIdx[0x37] + self->spriteIdx[0x38] * player;
       i < self->spriteIdx[0x37] + self->spriteIdx[0x38] * (player + 1); i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = self->spriteIdx[0x3f] + self->spriteIdx[0x40] * player;
       i < self->spriteIdx[0x3f] + self->spriteIdx[0x40] * (player + 1); i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
    UiEquipAlignGearRowSprite51de(
        self, player, (u8)(i - (self->spriteIdx[0x3f] + self->spriteIdx[0x40] * player)));
  }
  for (i = self->spriteIdx[0x43] + self->spriteIdx[0x44] * player;
       i < self->spriteIdx[0x43] + self->spriteIdx[0x44] * (player + 1); i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
  }
  for (i = self->spriteIdx[0x4d] + self->spriteIdx[0x4e] * player;
       i < self->spriteIdx[0x4d] + self->spriteIdx[0x4e] * (player + 1); i++) {
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
    ((GfxSprite **)self->base.data)[i]->posZ = self->spriteZ[i];
    UiEquipAlignGearRowSprite51fa(
        self, player, (u8)(i - (self->spriteIdx[0x4d] + self->spriteIdx[0x4e] * player)));
  }
}
