// bdc 0x089cbdd4 SysUtilHandlerIsEnabled
#include "bdc.h"

/* Returns the handler's enabled byte (+0xc); SysUtilPoll and SysUtilUpdateActive only act
   on enabled handlers. */
u8 SysUtilHandlerIsEnabled(SysUtilHandler *self)
{
    return self->enabled;
}
