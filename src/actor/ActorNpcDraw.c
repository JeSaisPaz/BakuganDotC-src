// bdc 0x088e5dec ActorNpcDraw
#include "bdc.h"

/* Draw method of the field NPC/guard classes (base `ActorNpcCtor`) (slot 8): draws
   (`ActorDraw`) only while the fade alpha `+0x6c` is positive. */

void ActorNpcDraw(ActorNpc *self, u32 **dl)

{
  if (!((self->base).base.ambient[3] <= 0.0f)) {
    ActorDraw(&self->base,dl);
  }
  return;
}

