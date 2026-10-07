// bdc 0x0882c11c UiGetTalkTask
#include "bdc.h"

/* `CoreTaskFind(0x6e)`: returns the talk/message window task, the `BtlHud` task object (id 0x6e),
   `this` of `UiTalkShowMessage`, `UiTalkSetMessageFile` and `UiTalkRequestClose`; callers
   check `UiTalkTaskExists` first. */

BtlHud *UiGetTalkTask(void)
{
  return CoreTaskFind(0x6e);
}
