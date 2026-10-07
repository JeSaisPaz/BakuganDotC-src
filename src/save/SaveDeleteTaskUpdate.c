// bdc 0x0880953c SaveDeleteTaskUpdate
#include "bdc.h"

/* Update of the savedata delete task (id 10030, `SaveDeleteTaskCtor`), a step machine on `step`:
   step 0 (once `SysUtilIsInit`) opens the savedata dialog (`SysUtilOpenDialog` kind 1), keeps
   its handler in `g_saveDeleteDialogSlot` and issues request 2 (the delete list, as in
   `SaveSaveTaskUpdate`'s out-of-space path) through the handler's virtual slot 2, then advances;
   step 1 waits for a handler and `SysUtilSavedataIsDone`, stores the result in profile word 1
   when a profile exists, closes the dialog and sets step 999. Any other step (negative or > 1)
   removes and destroys the task (`CoreTaskRemove`). */

void SaveDeleteTaskUpdate(CoreTask *task)

{
  SaveDeleteTask *self = (SaveDeleteTask *)task;
  SysUtilHandler *handler;
  const VtblEntry *entry;
  SaveProfile *profile;
  s32 step;

  step = self->step;
  if (step < 1) {
    if (step < 0) {
      CoreTaskRemove(task, true);
      return;
    }
    if (SysUtilIsInit()) {
      SysUtilOpenDialog(SysUtilGetCell(), 1);
      *g_saveDeleteDialogSlot = SysUtilGetDialog(SysUtilGetCell(), 1);
      handler = (SysUtilHandler *)*g_saveDeleteDialogSlot;
      entry = &handler->vtbl[2];
      ((void (*)(void *, s32))entry->fn)((char *)handler + entry->delta, 2);
      self->step++;
    }
  } else if (step > 1) {
    CoreTaskRemove(task, true);
    return;
  } else if (*g_saveDeleteDialogSlot != NULL && SysUtilSavedataIsDone()) {
    if (SaveHasProfile()) {
      profile = SaveGetProfile();
      SaveProfileSetWord(profile, 1, (u32)SysUtilSavedataGetResult());
    }
    SysUtilCloseDialog(SysUtilGetCell(), 1);
    self->step = 999;
  }
}
