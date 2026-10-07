// bdc 0x088779f0 BtlAttackGetRadius
#include "bdc.h"

/* Returns the top byte (byte 3, little-endian) of the attack type's info word in
   `g_btlAttackTypeInfo` as a float: the attack's radius/size (20..200 in the table). Used by the
   stage mine detection (`ActorStageObjMineDetect`). */
float BtlAttackGetRadius(BtlAttack *self)
{
    return (float)(u8)(g_btlAttackTypeInfo[self->type] >> 24);
}
