// bdc 0x089641b4 UiEquipUpdateGearPanelTween
#include "bdc.h"

/* Advances the equipment-panel tweens of the Bakugan/gear loadout screen before a battle (task 302,
   `UiEquipCtor`; per player picks Bakugan from a 20-face grid (`"baku_face_%02d"`) and equipment
   (`"c_setting_soubi_l_sol_%02d"`), shown on `"menu_daiza.gmo"` pedestals; `+0x4cda` = number of
   players (2 or 4 layouts), `+0x4cdb` = player being edited) started by
   `UiEquipStartGearPanelTween`: for each sprite group (first sprite `spriteIdx[g]`, per-player
   count `spriteIdx[g+1]`) runs `UiEquipUpdateSpriteFadeTween` on player `player`'s sprites, and
   after every group but the first (the anchor group 0x11) re-aligns that group to the anchor with
   `UiEquipAlignSpritesToAnchor` for `editPlayer`. Returns true when the 8-bit count of sprites
   whose tween reported done is nonzero (i.e. at least one finished, modulo 256). */

/* Fade every sprite of player `player` in group `g`; group bounds are re-read after each call. */
#define FADE_GROUP(g)                                                                        \
  for (i = self->spriteIdx[(g)] + self->spriteIdx[(g) + 1] * player;                        \
       i < self->spriteIdx[(g)] + self->spriteIdx[(g) + 1] * (player + 1); i++) {           \
    doneCount = (u8)(doneCount + UiEquipUpdateSpriteFadeTween(self, out, (u16)i));          \
  }

/* Fade group `g`, then align it to the anchor group (0x11/0x12) for the edited player. */
#define FADE_ALIGN_GROUP(g)                                                                  \
  FADE_GROUP(g)                                                                              \
  UiEquipAlignSpritesToAnchor(self, self->spriteIdx[0x11], self->spriteIdx[0x12],           \
                              self->spriteIdx[(g)], self->spriteIdx[(g) + 1],                \
                              (u8)self->editPlayer);

bool UiEquipUpdateGearPanelTween(UiEquip *self, bool out, u8 player)
{
  int i;
  u8 doneCount = 0;

  FADE_GROUP(0x11)
  FADE_ALIGN_GROUP(0x15)
  FADE_ALIGN_GROUP(0x1f)
  FADE_ALIGN_GROUP(0x21)
  FADE_ALIGN_GROUP(0x25)
  FADE_ALIGN_GROUP(0x27)
  FADE_ALIGN_GROUP(0x29)
  FADE_ALIGN_GROUP(0x2d)
  FADE_ALIGN_GROUP(0x31)
  FADE_ALIGN_GROUP(0x33)
  FADE_ALIGN_GROUP(0x37)
  FADE_ALIGN_GROUP(0x39)
  FADE_ALIGN_GROUP(0x3d)
  FADE_ALIGN_GROUP(0x3f)
  FADE_ALIGN_GROUP(0x43)
  FADE_ALIGN_GROUP(0x4d)
  return doneCount != 0;
}
