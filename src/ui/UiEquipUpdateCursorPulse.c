// bdc 0x08962f64 UiEquipUpdateCursorPulse
#include "bdc.h"

/* Pulses the highlighted cursor of the Bakugan/gear loadout screen before a battle (task 302,
   `UiEquipCtor`): `cursorPulse` grows by 0.2 per frame while below 1, and the cursor sprites are
   scaled by `1 + 0.2 * cursorPulse`, capped at 1.2. Grid mode (`onRandom` == 0): the four sprites of
   the highlighted grid cell (`spriteIdx[2]`, `[3]`, `[1]`, `[0x4b]` + `gridCursor`) are scaled, made
   visible again (flag 0x20 cleared) and put at depth -200..-203; `spriteIdx[6]`/`[7]` are scaled and
   put at -204/-205, and `spriteIdx[7]` is placed `rowGapA * its scaleY` above the cell's
   `spriteIdx[1]` sprite. Random mode: `spriteIdx[4]`, `[5]`, `[0xd]`, `[0xe]` are scaled and put at
   depth -200..-203. */

/* Layout sprite `i` of the screen's sprite table (re-read on every use, like the original). */
#define UI_EQUIP_SPRITE(i) (((GfxSprite **)self->base.data)[i])

void UiEquipUpdateCursorPulse(UiEquip *self)
{
  GfxSprite **sprites;
  float pulse;
  float scale;

  pulse = self->cursorPulse;
  sprites = (GfxSprite **)self->base.data;
  if (pulse < 1.0f) {
    pulse = pulse + 0.2f;
    self->cursorPulse = pulse;
  }
  scale = pulse * 0.20000005f + 1.0f;
  if (!(scale <= 1.2f)) {
    scale = 1.2f;
  }
  if (self->onRandom == 0) {
    UiSpriteSetScaleRotation(sprites[self->spriteIdx[2] + self->gridCursor], scale, scale, 0.0f);
    UI_EQUIP_SPRITE(self->spriteIdx[2] + self->gridCursor)->flags &= ~0x20u;
    UI_EQUIP_SPRITE(self->spriteIdx[2] + self->gridCursor)->posZ = -200.0f;

    UiSpriteSetScaleRotation(UI_EQUIP_SPRITE(self->spriteIdx[3] + self->gridCursor), scale, scale,
                             0.0f);
    UI_EQUIP_SPRITE(self->spriteIdx[3] + self->gridCursor)->flags &= ~0x20u;
    UI_EQUIP_SPRITE(self->spriteIdx[3] + self->gridCursor)->posZ = -201.0f;

    UiSpriteSetScaleRotation(UI_EQUIP_SPRITE(self->spriteIdx[1] + self->gridCursor), scale, scale,
                             0.0f);
    UI_EQUIP_SPRITE(self->spriteIdx[1] + self->gridCursor)->flags &= ~0x20u;
    UI_EQUIP_SPRITE(self->spriteIdx[1] + self->gridCursor)->posZ = -202.0f;

    UiSpriteSetScaleRotation(UI_EQUIP_SPRITE(self->spriteIdx[0x4b] + self->gridCursor), scale,
                             scale, 0.0f);
    UI_EQUIP_SPRITE(self->spriteIdx[0x4b] + self->gridCursor)->flags &= ~0x20u;
    UI_EQUIP_SPRITE(self->spriteIdx[0x4b] + self->gridCursor)->posZ = -203.0f;

    UiSpriteSetScaleRotation(UI_EQUIP_SPRITE(self->spriteIdx[6]), scale, scale, 0.0f);
    UI_EQUIP_SPRITE(self->spriteIdx[6])->posZ = -204.0f;

    UiSpriteSetScaleRotation(UI_EQUIP_SPRITE(self->spriteIdx[7]), scale, scale, 0.0f);
    UI_EQUIP_SPRITE(self->spriteIdx[7])->posZ = -205.0f;

    sprites = (GfxSprite **)self->base.data;
    sprites[self->spriteIdx[7]]->posY =
        sprites[self->spriteIdx[1] + self->gridCursor]->posY -
        self->rowGapA * sprites[self->spriteIdx[7]]->scaleY;
  } else {
    UiSpriteSetScaleRotation(sprites[self->spriteIdx[4]], scale, scale, 0.0f);
    UI_EQUIP_SPRITE(self->spriteIdx[4])->posZ = -200.0f;

    UiSpriteSetScaleRotation(UI_EQUIP_SPRITE(self->spriteIdx[5]), scale, scale, 0.0f);
    UI_EQUIP_SPRITE(self->spriteIdx[5])->posZ = -201.0f;

    UiSpriteSetScaleRotation(UI_EQUIP_SPRITE(self->spriteIdx[0xd]), scale, scale, 0.0f);
    UI_EQUIP_SPRITE(self->spriteIdx[0xd])->posZ = -202.0f;

    UiSpriteSetScaleRotation(UI_EQUIP_SPRITE(self->spriteIdx[0xe]), scale, scale, 0.0f);
    UI_EQUIP_SPRITE(self->spriteIdx[0xe])->posZ = -203.0f;
  }
}

#undef UI_EQUIP_SPRITE
