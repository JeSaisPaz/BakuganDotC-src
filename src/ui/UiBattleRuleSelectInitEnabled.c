// bdc 0x089527f4 UiBattleRuleSelectInitEnabled
#include "bdc.h"

/* Initialises the enabled flags of `UiBattleRuleSelect`: the four rule
   buttons `+0x5f9..+0x5fc` and the three sub-options `+0x5fd..+0x5ff` all 1, except that in network
   mode (`SaveGetProfileFlag0`) buttons 1 and 3 are disabled. */

void UiBattleRuleSelectInitEnabled(UiBattleRuleSelect *self)
{
  s8 flags[8];
  int i;

  flags[0] = 1;
  flags[1] = 1;
  flags[2] = 1;
  flags[3] = 1;
  flags[4] = 1;
  flags[5] = 1;
  flags[6] = 1;
  if (SaveGetProfileFlag0() != 0) {
    flags[1] = 0;
    flags[3] = 0;
  }
  for (i = 0; i < 4; i++) {
    self->buttonEnabled[i] = flags[i];
  }
  for (i = 0; i < 3; i++) {
    self->subEnabled[i] = flags[4 + i];
  }
}
