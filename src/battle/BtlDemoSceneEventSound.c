// bdc 0x08906ab0 BtlDemoSceneEventSound
#include "bdc.h"

/* Sound event handler of the battle demo scene player task (`BtlDemoScenePlayer`): when the
   event's frame equals the player's current frame, then unless the event's `flags` is nonzero
   it maps the cue `paramB` to a sound id (`BtlDemoSceneMapSoundId`) and, when that is not -1
   and the sound manager exists, plays it (`SndManagerPlay` with category 0, flag 0). It then
   deletes the event through its deleting destructor (vtable slot 1, flag 3) when the event is
   not NULL. */

void BtlDemoSceneEventSound(void *task, void *ev)
{
    BtlDemoScenePlayer *player = (BtlDemoScenePlayer *)task;
    BtlDemoScbSoundEvent *sound = (BtlDemoScbSoundEvent *)ev;
    s32 soundId;

    if (sound->base.frame != player->frame) {
        return;
    }
    if (sound->base.flags == 0) {
        soundId = BtlDemoSceneMapSoundId(task, sound->paramB);
        if (soundId != -1 && SndHasManager()) {
            SndManagerPlay(SndGetManager(), (u32)soundId, 0, 0);
        }
    }
    if (sound != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)sound->base.base.vtable)[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)sound + dtor->delta, 3);
    }
}
