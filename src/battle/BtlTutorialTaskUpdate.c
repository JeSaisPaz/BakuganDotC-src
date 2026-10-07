// bdc 0x08846e28 BtlTutorialTaskUpdate
#include "bdc.h"

/* Update of task id 104: while the battle camera task exists (`BtlCameraTaskExists`) and the
   battle is still undecided (`g_btlBattleOutcome` `== 0`) runs the tutorial step handler
   (`BtlTutorialStep`); otherwise removes itself (`CoreTaskRemove`). */

void BtlTutorialTaskUpdate(CoreTask *task)
{
    bool running;

    running = false;
    if (BtlCameraTaskExists() != 0 && g_btlBattleOutcome == 0) {
        running = true;
    }
    if (running) {
        BtlTutorialStep(task);
    } else {
        CoreTaskRemove(task, true);
    }
}
