// bdc 0x0892c40c UiBakuganEvolve
#include "bdc.h"

/* Evolves Bakugan `id` into its evolved form (`UiBakuganGetPair``(0, id)`; nothing happens
   without one) in the save profile (`SaveGetProfile`): if the evolved form is not owned yet
   (`ownedBakugan`), copies every upgrade `id` owns (`upgradeOwned[id][0..5]`, see
   `UiUpgradeApplyPurchase`) to it; marks `id` as evolved (bitset `ownedItems + 11`, profile
   `+0x525`), the evolved form as owned (`ownedBakugan`) and in `bakuganBitsA`/`bakuganBitsB`; makes
   it the current Bakugan when `id` was (`curBakugan`); and unlocks its gear item
   `form*4 + table[form] - 4` (table `g_uiBakuganEvolveGearIndex`, copied to the stack): when not
   yet in `ownedItems` it is set there and recorded in the first free (0xff) of the two
   `equipSlots[item/4 + 1]` bytes; the item is then always marked in `newItems`/`newItemGroups`. */

void UiBakuganEvolve(u8 id)

{
  u8 gearIndex[24];
  int form;
  int slot;
  int item;
  SaveProfile *profile;
  u8 bits;
  u8 *equip;

  memcpy(gearIndex, g_uiBakuganEvolveGearIndex, 0x15);
  form = UiBakuganGetPair(0, id);
  if (form == 0) {
    return;
  }
  if ((u8)(SaveGetProfile()->data->ownedBakugan[form / 8] & (1 << (form % 8))) == 0) {
    for (slot = 0; slot < 6; slot++) {
      if (SaveGetProfile()->data->upgradeOwned[id][slot] != 0) {
        SaveGetProfile()->data->upgradeOwned[form][slot] = 1;
      }
    }
  }
  SaveGetProfile()->data->ownedItems[11 + id / 8] |= (u8)(1 << (id % 8));
  profile = SaveGetProfile();
  profile->data->ownedBakugan[form / 8] |= (u8)(1 << (form % 8));
  profile->data->bakuganBitsA[form / 8] |= (u8)(1 << (form % 8));
  profile->data->bakuganBitsB[form / 8] |= (u8)(1 << (form % 8));
  if (SaveGetProfile()->data->curBakugan == id) {
    SaveGetProfile()->data->curBakugan = form;
  }
  profile = SaveGetProfile();
  item = form * 4 + gearIndex[form] - 4;
  bits = profile->data->ownedItems[item / 8];
  if ((u8)(bits & (1 << (item % 8))) == 0) {
    profile->data->ownedItems[item / 8] = bits | (u8)(1 << (item % 8));
    equip = profile->data->equipSlots[item / 4 + 1];
    if (equip[0] == 0xff) {
      equip[0] = (u8)item;
    }
    else if (equip[1] == 0xff) {
      equip[1] = (u8)item;
    }
  }
  profile->data->newItems[item / 8] |= (u8)(1 << (item % 8));
  profile->data->newItemGroups[item / 8] |= (u8)(1 << (item % 8));
}
