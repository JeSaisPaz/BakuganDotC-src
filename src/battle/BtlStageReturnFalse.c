// bdc 0x0889d9bc BtlStageReturnFalse
#include "bdc.h"

/* Always returns 0. Called by `UiTalkShowMessage` and `UiTalkWindowStep` as a stage/event
   query that is stubbed out in this build. */

int BtlStageReturnFalse(void)
{
    return 0;
}
