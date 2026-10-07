// bdc 0x08865288 BtlBakuganStartHitShake
#include "bdc.h"

/* Starts the hit shake of `BtlBakuganUpdateHitShake`: intensity 1.0, amplitude 0.5, direction
   `dir[0]`/`dir[2]` (x/z), decay per frame `decay`. Called by `BtlBakuganOnHit`. */
void BtlBakuganStartHitShake(float decay, BtlBakugan *bakugan, float *dir)
{
    bakugan->hitShake = 1.0f;
    bakugan->hitShakeAmplitude = 0.5f;
    bakugan->hitShakeDirX = dir[0];
    bakugan->hitShakeDirZ = dir[2];
    bakugan->hitShakeDecay = decay;
}
