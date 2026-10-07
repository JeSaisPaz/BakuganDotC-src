// bdc 0x089ee13c UiSpriteMngEnsureTask
#include "bdc.h"

/* Creates the 2D sprite manager task (core task id `0x276a`, priority 100 via `CoreTaskCreate`) if
   it does not exist yet. Called by `ScriptOpSprite` when its first command finds no manager. */

void UiSpriteMngEnsureTask(void)

{
  void *task;
  
  task = CoreTaskFind(0x276a);
  if (task == (void *)0x0) {
    CoreTaskCreate(0x276a,100);
  }
  return;
}

