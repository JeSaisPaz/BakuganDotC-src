// bdc 0x08883788 BtlAttackTypeUsesStageEffects
#include "bdc.h"

/* Returns 1 for attack types 0x47..0x57 and 0x7e..0x88, which spawn their effects on the stage
   effect manager instead of the battle attack manager; else 0. */
int BtlAttackTypeUsesStageEffects(void *owner, int type)
{
    (void)owner;
    if (type < 0x58) {
        return (type > 0x46) ? 1 : 0;
    }
    return (type > 0x7d && type < 0x89) ? 1 : 0;
}
