// bdc 0x088eb4c8 GameEventCtor
#include "bdc.h"

/* Base constructor of the field event (talk/cutscene command) task class: runs `CoreTaskInit`,
   installs the base vtable `g_gameEventVtbl`, clears the command/message state (the two 0x100-byte
   message buffers, the 16-byte script block at `+0x10`, flag bits 0, 1 and 3 of `flags`; bit 2 is
   set), sets `mode` 2, `bgmTrack` 0xff and the default window colours (0xf0) and returns the task.
   Its only derived class is task 470 (`GameEvent470Ctor`). */

GameEvent *GameEventCtor(GameEvent *self)

{
  CoreTaskInit(&self->base);
  self->base.vtable = g_gameEventVtbl;
  self->cmd = NULL;
  self->fades = NULL;
  self->fadeWork = NULL;
  self->camKeys = NULL;
  self->props = NULL;
  self->msgWindow = NULL;
  self->reserved38 = 0;
  self->actions = NULL;
  self->msgRequest = 0;
  self->msgRequestState = 0;
  self->msgRequestTimer = 0;
  self->iconPos = 0;
  self->cmdIndex = 0;
  self->cmdEnd = 0;
  self->msgCloseDelay = 0;
  self->msgPos = 0;
  self->msgCount = 0;
  self->msgIdleTimer = 0;
  self->msgAutoTimer = 0;
  self->msgAutoTime = 0;
  self->waitFrames = 0;
  self->propMotion = 0;
  self->state = 0;
  self->runStep = 0;
  self->coordMode = 0;
  self->flags &= ~0x01;
  self->mode = 2;
  self->waitType = 0;
  self->msgState = 0;
  self->windowColor[0] = 0xf0;
  self->windowColor[1] = 0xf0;
  self->reserved26d[0] = 0;
  self->reserved26d[1] = 0;
  self->reserved26d[2] = 0;
  self->flags &= ~0x02;
  self->msgAdvanceLock = 0;
  self->bgmTrack = 0xff;
  self->blocking = 0;
  self->flags |= 0x04;
  self->flags &= ~0x08;
  memset(&self->script, 0, 0x10);
  memset(self->msgIds, 0, sizeof(self->msgIds));
  memset(self->msgAttrs, 0, sizeof(self->msgAttrs));
  return self;
}
