// bdc 0x08907d20 BtlDemoScbEventGroupParse
#include "bdc.h"

/* Parses the records of a `.scb` event group: determines its type from the high and
   low bytes of the header half-word data[1] (BtlDemoScbEventGroupType) and creates the
   events with BtlDemoScbEventGroupReadEvents. */
void BtlDemoScbEventGroupParse(void *group, u16 *data)
{
    int type;

    type = BtlDemoScbEventGroupType(group, (u8)(data[1] >> 8), (u8)data[1]);
    BtlDemoScbEventGroupReadEvents(group, data, type);
}
