// bdc 0x08967470 UiEquipApplyPeerSelections
#include "bdc.h"

/* Network helper of the Bakugan/gear loadout screen before a battle (task 302,
   `maybe_UiScreen302Ctor`): steps the three per-player state machines of the four remote-player
   records `self->peer[i]` (`+0x5200 + i*0x28`).
   - `modelState` (+0x18): 1 = free the slot model (`UiEquipFreeSlotModel`), show the gauge
     (`UiEquipShowPlayerGauge` hide = 1) and restart the stamp with Bakugan 0
     (`UiEquipStartPlayerStamp`), then go to 2 if a Bakugan is picked, else 0; 2 = show the gauge,
     load the model (`UiEquipLoadSlotModel`), set its `ambient[3]` to 1.0, restart the stamp with
     the Bakugan, back to 0; 0 with a model loaded and a Bakugan 1..20 = pick texture variant `i` if
     an earlier player has the same Bakugan (else 0) and apply it (`GfxModelApplyTextureVariant`)
     when it changed.
   - `markState` (+0x20): 0 → 1 when the peer's `extra10` is 0x18, starting the "done" mark appear
     tween (`UiEquipStartPlayerDoneMarkTween`); 1 → 2 when the tween ends
     (`UiEquipUpdatePlayerDoneMarkTween`); 2 → 3 when `extra10` is no longer 0x18; 3 → 0 when the
     disappear tween ends.
   - `gearChanged` (+0x24): 0 → 1 when `extra10` is 0x18, starting the commit tween
     (`UiEquipStartCommitTween`); 1 → 2 when `UiEquipUpdateCommitTween` returns true; 2 → 3 when
     `extra10` is no longer 0x18; 3 → 0 hiding the player's rows (`UiEquipHidePlayerRows`). */

void UiEquipApplyPeerSelections(UiEquip *self)
{
  int i;
  int j;
  int bakugan;
  int variant;
  u8 slot;

  for (i = 0; i < 4; i++) {
    slot = (u8)i;
    switch (self->peer[i].modelState) {
    case 0:
      if (self->bakuganModels[i] != NULL) {
        bakugan = (s32)self->peer[i].bakugan;
        if (bakugan > 0 && bakugan < 21) {
          variant = 0;
          for (j = 0; j < i; j++) {
            if ((s32)self->peer[j].bakugan == bakugan) {
              variant = i;
            }
          }
          if (self->peer[i].variant != variant) {
            self->peer[i].variant = variant;
            GfxModelApplyTextureVariant(self->bakuganModels[i], variant);
          }
        }
      }
      break;
    case 1:
      UiEquipFreeSlotModel(self, slot);
      UiEquipShowPlayerGauge(self, true, slot, (u8)self->peer[i].bakugan);
      UiEquipStartPlayerStamp(self, slot, 0);
      if (self->peer[i].bakugan != 0) {
        self->peer[i].modelState = self->peer[i].modelState + 1;
      } else {
        self->peer[i].modelState = 0;
      }
      break;
    case 2:
      UiEquipShowPlayerGauge(self, false, slot, (u8)self->peer[i].bakugan);
      UiEquipLoadSlotModel(self, slot, (u8)self->peer[i].bakugan);
      self->bakuganModels[i]->ambient[3] = 1.0f;
      UiEquipStartPlayerStamp(self, slot, (u8)self->peer[i].bakugan);
      self->peer[i].modelState = 0;
      break;
    default:
      break;
    }

    switch (self->peer[i].markState) {
    case 0:
      if (self->peer[i].extra10 == 0x18) {
        self->peer[i].markState = 1;
        UiEquipStartPlayerDoneMarkTween(self, 0, slot);
      }
      break;
    case 1:
      if (UiEquipUpdatePlayerDoneMarkTween(self, 0, slot)) {
        self->peer[i].markState = self->peer[i].markState + 1;
      }
      break;
    case 2:
      if (self->peer[i].extra10 != 0x18) {
        self->peer[i].markState = 3;
      }
      break;
    case 3:
      if (UiEquipUpdatePlayerDoneMarkTween(self, 1, slot)) {
        self->peer[i].markState = 0;
      }
      break;
    default:
      break;
    }

    switch (self->peer[i].gearChanged) {
    case 0:
      if (self->peer[i].extra10 == 0x18) {
        UiEquipStartCommitTween(self, slot);
        self->peer[i].gearChanged = self->peer[i].gearChanged + 1;
      }
      break;
    case 1:
      if (UiEquipUpdateCommitTween(self, slot)) {
        self->peer[i].gearChanged = self->peer[i].gearChanged + 1;
      }
      break;
    case 2:
      if (self->peer[i].extra10 != 0x18) {
        self->peer[i].gearChanged = 3;
      }
      break;
    case 3:
      UiEquipHidePlayerRows(self, slot);
      self->peer[i].gearChanged = 0;
      break;
    default:
      break;
    }
  }
}
