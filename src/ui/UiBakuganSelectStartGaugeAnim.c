// bdc 0x0892e1fc UiBakuganSelectStartGaugeAnim
#include "bdc.h"

/* Arms the gauge animation record `+0x1c94` of the Bakugan select screen (`UiBakuganSelectCtor`,
   task 371; cursor `+0x74`, current entry `+0x75`, owned list `+0x1ba4` with 0xc-byte entries) for
   entry `entry`: for each of the four gauges stores the current value as start (`+0x1c95`), the
   target `10 * entry byte (+0x1ba8 + i)` (`+0x1c9d`) and the delta (`+0x1ca8`), resetting `t`
   `+0x1ca4`. */

void UiBakuganSelectStartGaugeAnim(UiBakuganSelect *self, u8 on, u32 entry)
{
  int i;

  self->gaugeOn = on;
  self->gaugeT = 0.0f;
  for (i = 0; i < 4; i++) {
    u8 value = ((u8 *)self->entries[entry & 0xff].display)[i];
    self->gaugeVals[i] = self->gaugeVals[i + 4];
    self->gaugeVals[i + 8] = value * 10;
    ((float *)self->gaugeDeltaRaw)[i] = (float)(int)((u32)self->gaugeVals[i + 8] - (u32)self->gaugeVals[i]);
  }
}
