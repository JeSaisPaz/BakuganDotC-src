// bdc 0x089d0a3c NetCharaGetReadyFrames
#include "bdc.h"

/* Returns the character's count of ready lock-step frames (built by `NetCharaSyncFrames`). */
s32 NetCharaGetReadyFrames(NetChara *self)
{
    return self->readyFrames;
}
