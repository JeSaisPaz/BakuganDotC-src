// bdc 0x088d51dc GameGimmickCollidableStateHit
#include "bdc.h"

/* State 1 handler of the breakable container gimmick (`GameGimmickCollidableCtor`, vtables
   `0x08af2e2c`/`0x08af2ecc`) (member table `0x08a96a00`; state 0 is idle `0x088d51d4`): unless
   already broken (`+0x1e1`), plays the hit sound `0x2c00037`, restarts motion 0 (`GfxModelSwapMotionFrame`) at
   speed 1 (virtual `+0x34`) and returns to state 0. */

void GameGimmickCollidableStateHit(GameGimmickCollidable *obj)
{
  if (obj->effectActive == 0) {
    const VtblEntry *vt;

    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x2c00037, 0, 0);
    }
    GfxModelSwapMotionFrame((GfxModel *)obj, 0.0f);
    vt = &((const VtblEntry *)obj->base.base.base.vtable)[6];
    ((void (*)(void *, float))vt->fn)((u8 *)obj + vt->delta, 1.0f);
    obj->base.state = 0;
  }
}
