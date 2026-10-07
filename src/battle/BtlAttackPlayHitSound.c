// bdc 0x0887801c BtlAttackPlayHitSound
#include "bdc.h"

/* Plays the attack's hit sound `params.hitSound` at the attack's position `pos`
   (`BtlAttackPlaySound` with both flags 0). */
void BtlAttackPlayHitSound(BtlAttack *self)
{
    BtlAttackPlaySound(self, self->params.hitSound, self->pos, 0, 0);
}
