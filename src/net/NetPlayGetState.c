// bdc 0x0881b224 NetPlayGetState
#include "bdc.h"

/* Returns the NetPlay state: 0 idle, 1 connect, 2 lobby, 3 host launch, 4 scan,
   5 join wait, 6 join launch, 7 session, 8 abort. */
s32 NetPlayGetState(NetPlay *self)
{
    return self->state;
}
