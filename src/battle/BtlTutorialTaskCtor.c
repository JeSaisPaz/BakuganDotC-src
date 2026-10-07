// bdc 0x08846674 BtlTutorialTaskCtor
#include "bdc.h"

/* Constructor of the battle tutorial driver task `BtlTutorialTask` (task id 104):
   `CoreTaskInit`, installs `g_btlTutorialTaskVtbl`, sets bit 3 of `g_scriptGlobalBits`
   (`CoreBitsetSet`), clears `advancePhase`, `reserved18` and `stepTimer`, applies the allowed-
   action masks of tutorial step 0 (`BtlTutorialSetActionMasks`) and sets profile word 3 to 1
   (`SaveProfileSetWord`). Returns `task`. */
BtlTutorialTask *BtlTutorialTaskCtor(BtlTutorialTask *task)
{
    CoreTaskInit(&task->base);
    task->base.vtable = g_btlTutorialTaskVtbl;
    CoreBitsetSet(3, g_scriptGlobalBits);
    task->advancePhase = 0;
    task->reserved18 = 0;
    task->stepTimer = 0;
    BtlTutorialSetActionMasks(task, 0);
    SaveProfileSetWord(SaveGetProfile(), 3, 1);
    return task;
}
