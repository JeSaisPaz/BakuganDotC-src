// bdc 0x089c1ed4 SndObjectSetPosPtr
#include "bdc.h"

/* Sets the position source of a `SndObject` to `pos`. Emitters that
   `SndObjectAddEmitter` creates afterwards track the three floats at that address. */
void SndObjectSetPosPtr(SndObject *obj, float *pos)
{
    obj->src = pos;
}
