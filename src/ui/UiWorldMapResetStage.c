// bdc 0x0899c50c UiWorldMapResetStage
#include "bdc.h"

/* Resets the selected stage of `UiWorldMap` to 0 before the stage list opens. */
void UiWorldMapResetStage(UiWorldMap *self)
{
    self->stage = 0;
}
