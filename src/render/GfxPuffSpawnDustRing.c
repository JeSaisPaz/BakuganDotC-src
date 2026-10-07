// bdc 0x08828bf4 GfxPuffSpawnDustRing
#include "bdc.h"

/* Spawns a horizontal ring of 16 dust puffs (kind 1, `GfxPuffCreate`) around `pos`: each gets an
   outward velocity (`scaleX..Z`, +0x90) of 1.0–1.1 at angle i·2π/16, is pushed 4 velocity lengths
   out, tinted (0.9, 0.7, 0.5) with alpha 0.2, gets a random size 4–7 and `matrix` row 0 zeroed
   (bank C720). Used by `GfxEffectRunCommands` (e.g. landing dust).
   Bank constants: S703 (2/π, cancels the vrot quarter turn), S733 (1.0, `vrndf1` [1,2) → [0,1)),
   C720 (zero vector). */

void GfxPuffSpawnDustRing(const float *pos)
{
  s32 i;
  GfxPuff *puff;
  float size;
  float angle;
  float speed;
  float rnd;
  float pushX;
  float pushY;
  float pushZ;

  i = 0;
  do {
    puff = GfxPuffCreate(1, pos);
    angle = (float)i * 6.28318548f;
    angle = angle * 0.0625f;
    rnd = PlatformRandFloat12() - 1.0f;
    speed = rnd * 0.1f;
    speed = speed + 1.0f;
    /* velocity = (cos a, 0, sin a) * speed; lane 3 of the vrot result is 0 */
    puff->base.scaleX = __builtin_cosf(angle) * speed;
    puff->base.scaleY = 0.0f;
    puff->base.scaleZ = __builtin_sinf(angle) * speed;
    puff->base.angle = 0.0f;
    puff->base.tint[0] = 0.9f;
    puff->base.tint[1] = 0.7f;
    puff->base.tint[2] = 0.5f;
    puff->base.alpha = 0.2f;
    /* position.xyz += velocity * 4; posW kept */
    pushX = puff->base.scaleX * 4.0f;
    pushY = puff->base.scaleY * 4.0f;
    pushZ = puff->base.scaleZ * 4.0f;
    puff->base.posX = puff->base.posX + pushX;
    puff->base.posY = puff->base.posY + pushY;
    puff->base.posZ = puff->base.posZ + pushZ;
    /* matrix row 0 = C720 (zero) */
    puff->base.matrix[0] = 0.0f;
    puff->base.matrix[1] = 0.0f;
    puff->base.matrix[2] = 0.0f;
    puff->base.matrix[3] = 0.0f;
    rnd = PlatformRandFloat12() - 1.0f;
    size = rnd * 3.0f;
    size = size + 4.0f;
    puff->base.width = size;
    puff->base.height = size;
    puff->base.depth = size;
    i++;
    puff->base.maybe_sizeW = 0.0f;
  } while (i < 16);
}
