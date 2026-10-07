// bdc 0x08a128bc GmoImageCtor
#include "bdc.h"

/* In-place constructor of a `GmoTexTrack` (the 0x30-byte texture animation-track records
   carved by `GmoImagePlanTakeImages`; despite the name, not a `GmoImage`): one reference,
   `mode` 1, frame range `g_gmoImageDefaultRangeLow`..`g_gmoImageDefaultRangeHigh`
   (-1e6..+1e6), frame rate `g_gmoImageDefaultField1c` (12.0); clears the key/frame pointers
   and counts, `time`, `blend` and the four frame indices. Bytes `+0xf..+0x13` are left
   untouched. Returns `track` (a NULL `track` is returned as is). */
GmoTexTrack *GmoImageCtor(GmoTexTrack *track)
{
    if (track == NULL) {
        return track;
    }
    track->mode = 1;
    track->startFrame = g_gmoImageDefaultRangeLow;
    track->refCount = 1;
    track->endFrame = g_gmoImageDefaultRangeHigh;
    track->unused02 = 0;
    track->frameRate = g_gmoImageDefaultField1c;
    track->keys = NULL;
    track->frames = NULL;
    track->keyCount = 0;
    track->frameCount = 0;
    track->time = 0.0f;
    track->blend = 0.0f;
    track->frameA0 = 0;
    track->frameB0 = 0;
    track->frameA1 = 0;
    track->frameB1 = 0;
    return track;
}
