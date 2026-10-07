// bdc 0x088c35b8 GameFieldDrawSettingsNop
#include "bdc.h"

/* Empty draw handler of the field task (id 500, GameFieldCtor) for phase 5,
   GameFieldPhaseSettings: nothing is drawn while the settings screen is open. */
void GameFieldDrawSettingsNop(CoreTask *task)
{
    (void)task;
}
