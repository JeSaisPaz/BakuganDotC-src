// bdc 0x08859c14 ActorStopVexosBarrier
#include "bdc.h"

/* Turns the barrier effect started by `ActorStartVexosBarrier` off: tells the effect manager
   (`g_btlUnitEffectMgr`) to set state 2 on every effect owned by the actor (`GfxEffectSetStateOwned(mgr, -1, actor,
   2)`), destroys the object at `actor+0x6d0` through its vtable (slot `+0x0c`, flag 3) and clears
   the pointer, restores the body collider (`+0x20c`): hit timer 0, clears flag bits 1, 0x40, 4; then plays sound id
   `0x200261` through the sound manager when one exists. */

void ActorStopVexosBarrier(Actor *self)

{
  ActorCrystal *crystal = (ActorCrystal *)self;
  void *obj;
  SndManager *mgr;

  GfxEffectSetStateOwned(g_btlUnitEffectMgr,-1,self,2);
  obj = crystal->auxObject;
  if (obj != (void *)0x0) {
    const VtblEntry *e = (const VtblEntry *)((CoreNode *)obj)->vtable + 1;
    ((void (*)(void *, int))e->fn)((char *)obj + e->delta, 3);
    crystal->auxObject = (void *)0x0;
  }
  crystal->base.collider0->hitTimer = 0;
  crystal->base.collider0->flags = crystal->base.collider0->flags & ~1u;
  crystal->base.collider0->flags = crystal->base.collider0->flags & ~0x40u;
  crystal->base.collider0->flags = crystal->base.collider0->flags & ~4u;
  if (SndHasManager()) {
    mgr = SndGetManager();
    SndManagerPlay(mgr,0x200261,0,0);
  }
  return;
}
