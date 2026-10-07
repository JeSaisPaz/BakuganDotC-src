// bdc 0x0896250c UiEquipNetSetPeerBakugan
#include "bdc.h"

/* Applies a remote player's Bakugan pick on the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`): if it differs from the stored one in the remote-player records at `+0x5200 +
   player*0x28` (`+0` Bakugan, `+4/+8` equipment, `+0xc` handicap, `+0x10/+0x14` extra words,
   `+0x18` Bakugan-changed flag, `+0x24` equipment-changed counter), stores it as the player's pick
   `+0x4cdd[player]` and in `+0x5200`, sets the changed flag `+0x18` = 1 (clearing `+0x1c/+0x20`)
   and hides the player's label sprite `+0x5170 + player`. Ignores `player` outside 0..3. */

typedef struct UiEquipPeerRec {
  s32 bakugan;
  u8 pad04[0x14];
  s32 changed;
  s32 word1c;
  s32 word20;
  u8 pad24[4];
} UiEquipPeerRec;

void UiEquipNetSetPeerBakugan(UiEquip *self, s32 player, s32 bakuganId)
{
  UiEquipPeerRec *rec;
  GfxSprite **sprites;
  GfxSprite *sprite;

  if (player < 0 || player >= 4) {
    return;
  }
  rec = (UiEquipPeerRec *)self->remote[player];
  if (rec->bakugan == bakuganId) {
    return;
  }
  self->bakuganPick[player] = (u8)bakuganId;
  rec->bakugan = bakuganId;
  rec->changed = 1;
  rec->word1c = 0;
  rec->word20 = 0;
  sprites = (GfxSprite **)self->base.data;
  sprite = sprites[self->spriteIdx[8] + player];
  sprite->flags &= ~1u;
}
