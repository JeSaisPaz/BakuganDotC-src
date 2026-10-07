// bdc 0x0881b334 NetPlayClearSelectedHost
#include "bdc.h"

/* Clears the 'host selected' flag of the `NetPlay` manager. */
void NetPlayClearSelectedHost(NetPlay *self)
{
    self->hasSelectedHost = 0;
}
