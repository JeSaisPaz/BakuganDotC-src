// bdc 0x088e81a8 ActorNpcSwitchRobotDtor
#include "bdc.h"

/* Destructor of the switch robot (model 0x53, `ActorNpcSwitchRobotCtor`, vtable `g_actorNpcSwitchRobotVtbl`)
   slot 1: stops the two foot effects 0x4a (anchors `+0x440`, `+0x450`) and runs `ActorNpcDtor`.
    */

void ActorNpcSwitchRobotDtor(ActorNpcSwitchRobot *self, u32 flags)

{
  if (self != (ActorNpcSwitchRobot *)0x0) {
    (self->base).base.base.base.vtable = (void *)&g_actorNpcSwitchRobotVtbl;
    GfxEffectStopAttached(g_worldEffectMgr,0x4a,self->footPos);
    GfxEffectStopAttached(g_worldEffectMgr,0x4a,self->footPos + 1);
    (self->base).base.base.base.vtable = (void *)g_actorNpcRobotVtbl;
    ActorNpcDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

