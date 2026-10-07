// bdc 0x08904e9c BtlDemoSceneCtor
#include "bdc.h"

/* Constructor of a `.scb` demo scene (`BtlDemoScene`, 0x1c4 bytes; loaded by
   `BtlDemoSceneLoad`): initialises the object list (`BtlDemoSceneObjListInit`) and the event
   table list (`BtlDemoSceneEventListInit`), clears `buffer` and `index`, then zeroes the first
   key block (0x60 bytes) and each of the five other key blocks (0x40 bytes) with `memset`.
   Returns `scene`. */
void *BtlDemoSceneCtor(void *scene)
{
    BtlDemoScene *self = (BtlDemoScene *)scene;
    s32 i;

    BtlDemoSceneObjListInit(&self->objects);
    BtlDemoSceneEventListInit(&self->events);
    self->buffer = NULL;
    self->index = 0;
    memset(self->track0, 0, sizeof(self->track0));
    for (i = 0; i < 5; i++) {
        memset(self->tracks[i], 0, sizeof(self->tracks[i]));
    }
    return scene;
}
