// bdc 0x0895c994 UiEquipSaveSelections
#include "bdc.h"

/* Writes the result of the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`) when it
   closes. If `gearEditing` is clear: menu result 1 (`UiSetMenuResult`),
   `UiEquipStoreSelectionWords`, then for each player the two chosen gear picks (`gearPick[p]`, 0xff =
   none; both slots are first reset to 0xff) into profile words 0x36 + p*2 + k and the handicap
   `handicap[p]` into word 0x46 + p; marks the picks as used (`UiEquipMarkSelectionsUsed`) and calls
   `SaveProfileSetMapMode(0, g_scriptGlobalVars[1])`. Otherwise: menu result 0. */

void UiEquipSaveSelections(UiEquip *self)
{
  SaveProfile *profile;
  s32 player;
  s32 k;

  if (self->gearEditing == 0) {
    UiSetMenuResult(&self->base, 1);
    UiEquipStoreSelectionWords(self);
    for (player = 0; player < self->playerCount; player++) {
      for (k = 0; k < 2; k++) {
        profile = SaveGetProfile();
        if (profile->words != NULL) {
          profile->words[0x36 + player * 2 + k] = 0xff;
        }
      }
      for (k = 0; k < 2; k++) {
        if (self->gearPick[player][k] != 0xff) {
          profile = SaveGetProfile();
          if (profile->words != NULL) {
            profile->words[0x36 + player * 2 + k] = self->gearPick[player][k];
          }
        }
      }
      profile = SaveGetProfile();
      if (profile->words != NULL) {
        profile->words[0x46 + player] = self->handicap[player];
      }
    }
    UiEquipMarkSelectionsUsed(self);
    SaveProfileSetMapMode(0, (u8)g_scriptGlobalVars[1]);
  }
  else {
    UiSetMenuResult(&self->base, 0);
  }
}
