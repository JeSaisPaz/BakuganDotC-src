// bdc 0x089655a0 UiEquipUpdateGearCursorPulse
#include "bdc.h"

/* Pulses the highlighted equipment-row entry of player `player` on the Bakugan/gear loadout screen
   before a battle (task 302, `UiEquipCtor`), like `UiEquipUpdateCursorPulse` for the grid:
   `cursorPulse` grows by 0.2 per frame while below 1, and the sprites are scaled by
   `1 + 0.2 * cursorPulse`, capped at 1.2. The row is `activeRow`, or `gearRowFlags[localPlayer]`
   when profile flag 0 is set. Row 0 (equipment list): the seven sprites of entry `panelEntry` of
   the player (index `spriteIdx[k] + spriteIdx[k+1] * player + panelEntry`) are scaled and put at
   depth -200..-205/-207 (the -202 one is also made visible, flag 0x20 cleared), the two row-label
   sprites are re-aligned (`UiEquipAlignGearRowSprite51de`, `UiEquipAlignGearRowSprite51fa`),
   and `spriteIdx[0x2f] + player` is put at -206. Row 1 (OK button): three sprites of the player
   are scaled and put at -200..-202. Any other row: nothing is scaled. */

/* Layout sprite `i` of the screen's sprite table (re-read on every use, like the original). */
#define UI_EQUIP_SPRITE(i) (((GfxSprite **)self->base.data)[i])

/* Scales sprite `idx` and sets its depth. */
#define UI_EQUIP_PULSE(idx, z)                                         \
  do {                                                                 \
    UiSpriteSetScaleRotation(UI_EQUIP_SPRITE(idx), scale, scale, 0.0f); \
    UI_EQUIP_SPRITE(idx)->posZ = (z);                                  \
  } while (0)

void UiEquipUpdateGearCursorPulse(UiEquip *self, u8 player)
{
  float pulse;
  float scale;
  s8 row;
  u16 idx;

  pulse = self->cursorPulse;
  if (pulse < 1.0f) {
    pulse = pulse + 0.2f;
    self->cursorPulse = pulse;
  }
  scale = pulse * 0.20000005f + 1.0f;
  if (!(scale <= 1.2f)) {
    scale = 1.2f;
  }
  row = self->activeRow;
  if (SaveGetProfileFlag0() != 0) {
    row = self->gearRowFlags[*(s32 *)self->localPlayer];
  }
  if (row == 0) {
    idx = self->spriteIdx[0x25] + self->spriteIdx[0x26] * player + self->panelEntry;
    UI_EQUIP_PULSE(idx, -200.0f);
    idx = self->spriteIdx[0x27] + self->spriteIdx[0x28] * player + self->panelEntry;
    UI_EQUIP_PULSE(idx, -201.0f);
    idx = self->spriteIdx[0x2d] + self->spriteIdx[0x2e] * player + self->panelEntry;
    UI_EQUIP_PULSE(idx, -203.0f);
    idx = self->spriteIdx[0x37] + self->spriteIdx[0x38] * player + self->panelEntry;
    UI_EQUIP_PULSE(idx, -202.0f);
    UI_EQUIP_SPRITE(idx)->flags &= ~0x20u;
    idx = self->spriteIdx[0x3f] + self->spriteIdx[0x40] * player + self->panelEntry;
    UI_EQUIP_PULSE(idx, -205.0f);
    UiEquipAlignGearRowSprite51de(self, player, (u8)self->panelEntry);
    idx = self->spriteIdx[0x43] + self->spriteIdx[0x44] * player + self->panelEntry;
    UI_EQUIP_PULSE(idx, -204.0f);
    idx = self->spriteIdx[0x4d] + self->spriteIdx[0x4e] * player + self->panelEntry;
    UI_EQUIP_PULSE(idx, -207.0f);
    UiEquipAlignGearRowSprite51fa(self, player, (u8)self->panelEntry);
    UI_EQUIP_PULSE(self->spriteIdx[0x2f] + player, -206.0f);
  } else if (row == 1) {
    idx = self->spriteIdx[0x1f] + self->spriteIdx[0x20] * player;
    UI_EQUIP_PULSE(idx, -200.0f);
    idx = self->spriteIdx[0x21] + self->spriteIdx[0x22] * player;
    UI_EQUIP_PULSE(idx, -201.0f);
    UI_EQUIP_PULSE(self->spriteIdx[0x23] + player, -202.0f);
  }
}
