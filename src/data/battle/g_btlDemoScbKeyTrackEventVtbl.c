// bdc 0x08af478c g_btlDemoScbKeyTrackEventVtbl
#include "bdc.h"

__typeof__(VtblEntry[3]) g_btlDemoScbKeyTrackEventVtbl = {
    {0}, { .fn = (void *)BtlDemoScbKeyTrackEventDtor },
    { .fn = (void *)BtlDemoScbKeyTrackEventParse },
};
