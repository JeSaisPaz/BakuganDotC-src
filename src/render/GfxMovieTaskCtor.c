// bdc 0x089d68b8 GfxMovieTaskCtor
#include "bdc.h"

/* Constructor of the PSMF movie task (task id 10040 = 0x2738, 0x1c bytes, vtable `0x08af533c`;
   started/stopped by `ScriptOpPlayMovie` together with
   `GfxMovieRequestPlay`/`GfxMovieIsDone`): resets the movie request globals (`0x08ac5b94` movie
   id = -1, `0x08ac5b98`, `0x08ac5b9c` play state = 0, `0x08ac5b88..90`), clears its skip state
   (`+0x10` skipping, `+0x14` fade counter -1, `+0x18` alpha 1.0), sets a black clear colour and
   switches sound decoder 0 to mode 6 (`SndDecOutSetMode`). */

CoreTask *GfxMovieTaskCtor(CoreTask *task)

{
  GfxMovieTask *self;
  SndDecOut *dec;

  self = (GfxMovieTask *)task;
  CoreTaskInit(task);
  task->vtable = g_gfxMovieTaskVtbl;
  g_movieRequestId = 0xffffffff;
  g_movieRequestBusy = '\0';
  self->skipping = 0;
  self->fadeCounter = -1;
  self->alpha = 1.0f;
  g_movieRequestState = 0;
  g_gfxDisplay->clearColor[0] = 0.0;
  g_gfxDisplay->clearColor[1] = 0.0;
  g_gfxDisplay->clearColor[2] = 0.0;
  g_gfxDisplay->clearColor[3] = 1.0;
  g_movieStateA = '\0';
  g_movieStateB = 0;
  g_movieStateC = 0;
  dec = SndDecOutGet(0);
  SndDecOutSetMode(dec,6);
  return task;
}

