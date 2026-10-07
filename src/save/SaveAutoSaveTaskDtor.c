// bdc 0x0880b9bc SaveAutoSaveTaskDtor
#include "bdc.h"

/* Destructor of the autosave task (id 10022): deletes its text box (`UiTextBoxDelete`) and fader
   (virtual destructor at `+0x50`), copies the save data to the backup block
   (`SaveSnapshotUpdate`) when the save succeeded, chains to `CoreTaskDestroy`. */

void SaveAutoSaveTaskDtor(CoreTask *task, u32 flags)

{
  SaveAutoSaveTask *self = (SaveAutoSaveTask *)task;
  GfxFader *fader;
  const VtblEntry *entry;

  if (task != NULL) {
    task->vtable = &g_saveAutoSaveTaskVtbl;
    if (self->textBox != NULL) {
      UiTextBoxDelete(self->textBox,3);
      self->textBox = NULL;
    }
    fader = self->fader;
    if (fader != NULL) {
      entry = fader->vtbl + 1;
      ((void (*)(void *,int))entry->fn)((char *)fader + entry->delta,3);
      self->fader = NULL;
    }
    if (SaveProfileGetWord(SaveGetProfile(),1) == 1) {
      SaveSnapshotUpdate();
    }
    CoreTaskDestroy(task,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task,NULL,0);
      MemUnlock();
    }
  }
}
