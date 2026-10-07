// bdc 0x088ff400 BtlIsDialogBusy
#include "bdc.h"

/* Returns true when the message window exists and has not closed yet (`UiMsgWindowExists`,
   `UiMsgWindowGet`, `UiMsgWindowIsClosed`) or a task with id 0x2726 exists
   (`CoreTaskExists`), else false. Both checks always run. */

bool BtlIsDialogBusy(void)
{
    bool busy = false;

    if (UiMsgWindowExists()) {
        if (!UiMsgWindowIsClosed(UiMsgWindowGet())) {
            busy = true;
        }
    }
    if (CoreTaskExists(0x2726) != 0) {
        busy = true;
    }
    return busy;
}
