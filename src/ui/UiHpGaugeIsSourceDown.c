// bdc 0x0888b2c4 UiHpGaugeIsSourceDown
#include "bdc.h"

/* Returns whether the source of the HUD hit-point gauge (`UiHpGaugeInit`, 0xa0 bytes) is out:
   mode 1 → the unit's `BtlCombatState` `dead` byte (`unit+0x4c1`); mode 2 → the object's
   destroyed byte `+0x281`, forced to 1 while the battle flag byte `0x08aba77d` is set; 0 otherwise.
   Used by `UiHpGaugeUpdate` and `UiHpGaugeDraw`. */

u8 UiHpGaugeIsSourceDown(UiHpGauge *self)
{
  s32 mode = self->mode;
  u8 result = 0;
  if (mode < 2) {
    if (0 < mode) {
      return (self->unit->combat).dead;
    }
  } else if (mode < 3) {
    result = self->object->dead;
    if (g_btlBattleOver != 0) {
      result = 1;
    }
  }
  return result;
}
