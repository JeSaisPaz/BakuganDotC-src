// bdc 0x08877020 BtlAttackPlaySound
#include "bdc.h"

/* Plays an attack sound, dropping -1 and a repeat of the last id `g_btlAttackSoundLastId` unless
   the frame counter `g_btlFrameCount` is more than one past `g_btlAttackSoundLastFrame`: with
   `a`/`b` both 0 creates a positional one-shot emitter at `pos` (or the attack position) via
   `SndEmitterCreateAtPos`; otherwise moves the shared attack sound object
   `g_btlAttackSndObject` there (`SndObjectSetPos`) and adds the sound to it
   (`SndObjectAddEmitter`). */

void BtlAttackPlaySound(BtlAttack *self, s32 sound, float *pos, u8 a, u8 b)
{
    if (sound == -1) {
        return;
    }
    if (g_btlAttackSoundLastId == sound && !(g_btlAttackSoundLastFrame + 1 < g_btlFrameCount)) {
        return;
    }
    g_btlAttackSoundLastId = sound;
    g_btlAttackSoundLastFrame = g_btlFrameCount;
    if (a == 0 && b == 0) {
        if (pos == NULL) {
            SndEmitterCreateAtPos(SndGetListener(), sound, self->pos, 0, 1);
        } else {
            SndEmitterCreateAtPos(SndGetListener(), sound, pos, 0, 1);
        }
    } else {
        if (pos == NULL) {
            SndObjectSetPos((SndObject *)g_btlAttackSndObject, self->pos[0], self->pos[1], self->pos[2]);
        } else {
            SndObjectSetPos((SndObject *)g_btlAttackSndObject, pos[0], pos[1], pos[2]);
        }
        SndObjectAddEmitter((SndObject *)g_btlAttackSndObject, sound, a, b);
    }
}
