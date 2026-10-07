// bdc 0x08899870 BtlAiComboTableGetInputs
#include "bdc.h"

/* Returns the s16 input array of entry `index` of the combo-input table of `BtlAi`
   (`ai+0x788`, 0x18c bytes: owner kind `+0`, up to 16 entries of 0x18 bytes from `+4` = `{s32
   group; s16 inputs[8]; s32 length}`, count `+0x184`, vtable `+0x188`) (`table + 8 + index ×
   0x18`). */

s16 *BtlAiComboTableGetInputs(BtlAiComboTable *table, s32 index)

{
  return table->entries[index].inputs;
}

