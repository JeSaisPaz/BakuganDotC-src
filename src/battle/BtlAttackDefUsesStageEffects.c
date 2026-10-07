// bdc 0x088837cc BtlAttackDefUsesStageEffects
#include "bdc.h"

/* `BtlAttackTypeUsesStageEffects` for the type in an attack definition (`def[2]`); 0 for NULL. */

int BtlAttackDefUsesStageEffects(void *owner, s16 *def)

{
  if (def == NULL) {
    return 0;
  }
  return BtlAttackTypeUsesStageEffects(owner, def[2]);
}
