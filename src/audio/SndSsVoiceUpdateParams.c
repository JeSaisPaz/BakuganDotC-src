// bdc 0x08a23850 SndSsVoiceUpdateParams
#include "bdc.h"

/* Before each SAS grain, pushes changed voice parameters to SAS for every voice with a non-zero
   state: when volume/pan/volume2/pan2 (`+0x60..+0x63`) differ from the applied copies (`+8..+0xb`)
   it recomputes the dry and effect L/R volumes (`SndSsPanToVolumes` with the tone volumes
   `+0x34`/`+0x3c` and pans `+0x38`/`+0x40`), applies them to SAS voice = table index
   (`SndSasSetVolume`, `SndSsStoreVoiceVolumes`) and updates the copies; when the pitch offset
   (`+0x64`) differs from the applied one (`+0xc`) it recomputes the pitch from the note
   (`SndSsNoteToPitch` × rate / 44100, or the raw pitch `+0x30` for note 0xff), adds the offset,
   clamps 1..0x4000, applies it to `sasVoice` (`SndSasSetPitch`) and records the offset. */

void SndSsVoiceUpdateParams(void)
{
    SndSsVoice *v;
    u32 i;
    u32 base;
    s32 pitch;
    u32 left;
    u32 right;
    u32 effectLeft;
    u32 effectRight;

    for (i = 0, v = g_sndSsVoices; i < g_sndSsVoiceCount; i++, v++) {
        if (v->state == 0) {
            continue;
        }
        if (v->volL != v->volume || v->panL != v->pan || v->volR != v->volume2 ||
            v->panR != v->pan2) {
            SndSsPanToVolumes(v->field34, v->volume, v->field38, v->pan, &left, &right);
            SndSsPanToVolumes(v->field3c, v->volume2, v->field40, v->pan2, &effectLeft,
                              &effectRight);
            SndSasSetVolume(i, left, right, effectLeft, effectRight);
            SndSsStoreVoiceVolumes(i, left, right, effectLeft, effectRight);
            v->volL = v->volume;
            v->panL = v->pan;
            v->volR = v->volume2;
            v->panR = v->pan2;
        }
        if (v->field0c != v->field64) {
            if (v->note == 0xff) {
                base = v->pitch;
            } else {
                u16 factor = SndSsNoteToPitch(v->centerNoteU, (s16)((v->fine * 127) / 100),
                                              v->note, (s16)((v->centerNote * 127) / 100));
                base = (v->sampleRate * factor) / 44100;
            }
            pitch = (s32)(base + v->field64);
            if (pitch > 0x4000) {
                pitch = 0x4000;
            }
            if (pitch < 1) {
                pitch = 1;
            }
            SndSasSetPitch(v->sasVoice, pitch);
            v->field0c = v->field64;
        }
    }
}
