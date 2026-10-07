// bdc 0x088779b8 BtlAttackUsesLinkedTarget
#include "bdc.h"

/* Returns whether the attack's type has the linked-target flag (high nibble of byte 2, bits 20..23,
   of its info word in `g_btlAttackTypeInfo`, the same flag `BtlAttackInit` uses to target the
   owner's linked unit). Used by `ActorStageObjMineDetect`. */
int BtlAttackUsesLinkedTarget(BtlAttack *self)
{
    return ((g_btlAttackTypeInfo[self->type] >> 20) & 0xf) != 0;
}
