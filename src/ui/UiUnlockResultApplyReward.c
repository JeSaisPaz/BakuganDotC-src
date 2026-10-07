// bdc 0x0893b068 UiUnlockResultApplyReward
#include "bdc.h"

/* Writes the reward shown by the unlock-result screen (task 375) into the save profile
   (`SaveGetProfile``()->data`), switching on `rewardKind` with the item index `rewardIndex`:
   0: nothing; 1: gear item — sets its `ownedItems` bit and, if newly owned, equips it into the
   first free slot of `equipSlots[item / 4 + 1]`, then marks it in `newItems`/`newItemGroups`;
   2: sets bit `(u8)index` of `helpSeen`; 3: increments the counter byte at `fieldCounter + index`
   (capped at 99); 4: adds `index` to `points` (clamped to 0..9999999); 5: sets bit `index + 1` of
   `specialBits` (byte offset `g_rewardMaxusSet`) if not yet set; 6: sets the bit of the figure
   `UiUnlockResultMapFigureIndex``(self, 1, index)` in the bitmap at `upgradeOwned` if not yet
   set; 7: adds 10 to `fieldCounter` (capped at 99), mirrors it into `g_gameEventFlags``[5]` and
   into byte 5 of the profile's `eventFlags` copy; 8: adds 10000 to `points` (clamped); 9: sets bit
   `index + 1` of `ownedBakugan`, `bakuganBitsA` and `bakuganBitsB`. Other kinds do nothing. */

void UiUnlockResultApplyReward(UiUnlockResult *self)
{
  union {
    u32 words[0x42];
    u8 bytes[0x108];
  } flags; /* stack copy of the profile's `eventFlags` */
  SaveProfile *profile;
  u8 *slots;
  u8 *figureBits;
  int item;
  int points;
  int i;
  s8 counter;

  switch (self->rewardKind) {
  case 0:
    break;
  case 1:
    profile = SaveGetProfile();
    item = self->rewardIndex;
    if ((u8)(profile->data->ownedItems[item / 8] & (1 << (item % 8))) == 0) {
      profile->data->ownedItems[item / 8] |= 1 << (item % 8);
      slots = profile->data->equipSlots[item / 4 + 1];
      if (slots[0] == 0xff)
        slots[0] = (u8)item;
      else if (slots[1] == 0xff)
        slots[1] = (u8)item;
    }
    profile->data->newItems[item / 8] |= 1 << (item % 8);
    profile->data->newItemGroups[item / 8] |= 1 << (item % 8);
    break;
  case 2:
    profile = SaveGetProfile();
    item = (u8)self->rewardIndex;
    profile->data->helpSeen[item / 8] |= 1 << (item % 8);
    break;
  case 3:
    /* The counter byte is addressed relative to `fieldCounter` by the reward index. */
    profile = SaveGetProfile();
    item = self->rewardIndex;
    (&profile->data->fieldCounter)[item]++;
    if ((&profile->data->fieldCounter)[item] >= 100)
      (&profile->data->fieldCounter)[item] = 99;
    break;
  case 4:
    profile = SaveGetProfile();
    points = profile->data->points + self->rewardIndex;
    if (points > 9999999)
      points = 9999999;
    else if (points < 0)
      points = 0;
    profile->data->points = points;
    break;
  case 5:
    profile = SaveGetProfile();
    item = self->rewardIndex + 1;
    if ((u8)(profile->data->specialBits[g_rewardMaxusSet + item / 8] & (1 << (item % 8))) == 0) {
      profile = SaveGetProfile();
      item = self->rewardIndex + 1;
      profile->data->specialBits[g_rewardMaxusSet + item / 8] |= 1 << (item % 8);
    }
    break;
  case 6:
    /* The mapped figure index is used as a bit index into the bytes starting at `upgradeOwned`. */
    profile = SaveGetProfile();
    item = UiUnlockResultMapFigureIndex(self, 1, (u8)self->rewardIndex);
    figureBits = &profile->data->upgradeOwned[0][0];
    if ((u8)(figureBits[item / 8] & (1 << (item % 8))) == 0) {
      profile = SaveGetProfile();
      item = UiUnlockResultMapFigureIndex(self, 1, (u8)self->rewardIndex);
      figureBits = &profile->data->upgradeOwned[0][0];
      figureBits[item / 8] |= 1 << (item % 8);
    }
    break;
  case 7:
    profile = SaveGetProfile();
    profile->data->fieldCounter += 10;
    if (profile->data->fieldCounter >= 100)
      profile->data->fieldCounter = 99;
    counter = SaveGetProfile()->data->fieldCounter;
    g_gameEventFlags[5] = counter < 100 ? counter : 99;
    profile = SaveGetProfile();
    for (i = 0; i < 0x21; i++) {
      flags.words[i * 2] = ((u32 *)profile->data->eventFlags)[i * 2];
      flags.words[i * 2 + 1] = ((u32 *)profile->data->eventFlags)[i * 2 + 1];
    }
    flags.bytes[5] = g_gameEventFlags[5];
    profile = SaveGetProfile();
    for (i = 0; i < 0x21; i++) {
      ((u32 *)profile->data->eventFlags)[i * 2] = flags.words[i * 2];
      ((u32 *)profile->data->eventFlags)[i * 2 + 1] = flags.words[i * 2 + 1];
    }
    break;
  case 8:
    profile = SaveGetProfile();
    points = profile->data->points + 10000;
    if (points > 9999999)
      points = 9999999;
    else if (points < 0)
      points = 0;
    profile->data->points = points;
    break;
  case 9:
    profile = SaveGetProfile();
    item = self->rewardIndex + 1;
    profile->data->ownedBakugan[item / 8] |= 1 << (item % 8);
    profile->data->bakuganBitsA[item / 8] |= 1 << (item % 8);
    profile->data->bakuganBitsB[item / 8] |= 1 << (item % 8);
    break;
  }
}
