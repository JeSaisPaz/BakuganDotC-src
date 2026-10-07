// bdc 0x0884bf5c BtlMainSetField
#include "bdc.h"

/* `SetField` override (vtable slot 5) of the main battle-scene task `BtlMain`: fields 0-2 go to
   `CoreTaskSetField`; 3, 4, 5 and 7 store `value != 0` in `fieldFlags[0..3]`, 10 stores
   `value != 0` in `field10`; 6 stores `value` in the battle outcome `g_btlBattleOutcome`; 8 and 11
   store `value` in `field8` / `field11`; 9 and every other field are ignored. */
void BtlMainSetField(BtlMain *self, u32 field, u32 value)
{
  if (field < 3) {
    CoreTaskSetField(&self->base, field, value);
    return;
  }
  switch (field) {
  case 3:
    self->fieldFlags[0] = value != 0;
    break;
  case 4:
    self->fieldFlags[1] = value != 0;
    break;
  case 5:
    self->fieldFlags[2] = value != 0;
    break;
  case 6:
    g_btlBattleOutcome = (s32)value;
    break;
  case 7:
    self->fieldFlags[3] = value != 0;
    break;
  case 8:
    self->field8 = value;
    break;
  case 10:
    self->field10 = value != 0;
    break;
  case 11:
    self->field11 = value;
    break;
  default:
    break;
  }
}
