// bdc 0x08809238 SaveAutoLoadTaskUpdate
#include "bdc.h"

/* Update of the save-data autoload task (id 10009), a step machine on `step`:
   step 0 (once `SysUtilIsInit`) opens the savedata dialog (`SysUtilOpenDialog` kind 1), keeps
   its handler and issues request 3 (autoload `PLAYDATA.BIN`) through the handler's virtual slot 2,
   starts stopwatch 5 and advances; step 1 waits for a handler, `SysUtilSavedataIsDone` and a
   profile, then reads the result (`SysUtilSavedataGetResult`), laps stopwatch 5, and if the
   result is 1 resets the profile (`SaveProfileReset`) and sets profile flag `0x40000000`, then
   advances; step 2 (once `SysUtilIsInit`) closes the dialog (`SysUtilCloseDialog`) and
   advances; any other step (negative or > 2) removes and destroys the task. */

void SaveAutoLoadTaskUpdate(CoreTask *task)

{
  SaveAutoLoadTask *self = (SaveAutoLoadTask *)task;
  SysUtilSavedataHandler *handler;
  const VtblEntry *entry;
  s32 result;

  switch (self->step) {
  case 0:
    if (SysUtilIsInit()) {
      SysUtilOpenDialog(SysUtilGetCell(), 1);
      handler = SysUtilGetDialog(SysUtilGetCell(), 1);
      self->handler = handler;
      entry = &handler->base.vtbl[2];
      ((void (*)(void *, s32))entry->fn)((char *)handler + entry->delta, 3);
      self->step++;
      CoreStopwatchStart(5);
    }
    break;
  case 1:
    if (self->handler != NULL && SysUtilSavedataIsDone() && SaveHasProfile()) {
      result = SysUtilSavedataGetResult();
      CoreStopwatchLap(5);
      if (result == 1) {
        SaveProfileReset(SaveGetProfile());
        SaveProfileSetFlags(SaveGetProfile(), 0x40000000);
      }
      self->step++;
    }
    break;
  case 2:
    if (SysUtilIsInit()) {
      SysUtilCloseDialog(SysUtilGetCell(), 1);
      self->step++;
    }
    break;
  default:
    CoreTaskRemove(task, true);
    break;
  }
}
