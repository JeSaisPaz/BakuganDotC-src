// bdc 0x089d6c3c GfxMovieTaskSlot3
#include "bdc.h"

/* Empty override of vtable slot 3 for the movie task (id 10040): does nothing (the base class
   uses `CoreTaskBaseSlot3Nop`). */
void GfxMovieTaskSlot3(CoreTask *task)
{
    (void)task;
}
