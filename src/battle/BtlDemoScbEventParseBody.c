// bdc 0x08a2cfd8 BtlDemoScbEventParseBody
#include "bdc.h"

/* Default body parser of a `.scb` event node (base vtable slot `+0x14`): reads nothing and
   returns 0 body bytes. */
int BtlDemoScbEventParseBody(void *ev, u16 *body)
{
    (void)ev;
    (void)body;
    return 0;
}
