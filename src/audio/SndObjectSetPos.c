// bdc 0x089c1edc SndObjectSetPos
#include "bdc.h"

/* Gives a sound object a position of its own: stores x, y, z in its inline
   posData and points the position source at them (SndObjectSetPosPtr), so
   emitters created afterwards sound from that fixed position. */
void SndObjectSetPos(SndObject *obj, float x, float y, float z)
{
    obj->posData[0] = x;
    obj->posData[1] = y;
    obj->posData[2] = z;
    SndObjectSetPosPtr(obj, obj->posData);
}
