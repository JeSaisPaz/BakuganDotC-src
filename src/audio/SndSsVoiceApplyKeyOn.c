// bdc 0x08a23648 SndSsVoiceApplyKeyOn
#include "bdc.h"

/* Programs a hardware voice from a queued key-on record `cmd` (a `SndSsVoice`-layout record, SAS
   voice in `sasVoice`): sets the sample address/size (looping) or, when `field1c` is set, the noise
   clock (`SndSasSetVoice` / `SndSasSetNoise`); sets the pitch (`SndSasSetPitch`) either to the
   record's raw `pitch` (voice note 0xff, no offset or clamp) or from the voice table entry's note
   (`SndSsNoteToPitch` × sample rate / 44100 + pitch offset, clamped 1..0x4000) and records the
   applied offset in the table entry; computes the dry/effect volumes (`SndSsPanToVolumes`),
   applies them (`SndSasSetVolume`), the two ADSR words (`SndSasSetSimpleADSR`), stores the
   volumes (`SndSsStoreVoiceVolumes`), copies volume/pan to the applied copies and keys the voice
   on (`SndSasKeyOn`). Returns 0, or -1 when the key-on fails. Called by `SndSsVoiceQueueKeyOn`. */

s32 SndSsVoiceApplyKeyOn(u8 *cmd)
{
    SndSsVoice *rec = (SndSsVoice *)cmd;
    SndSsVoice *v;
    u32 idx;
    s32 pitch;
    u32 left;
    u32 right;
    u32 effectLeft;
    u32 effectRight;

    if (rec->field1c == 0) {
        SndSasSetVoice(rec->sasVoice, rec->field20, rec->field24, 1);
    } else {
        SndSasSetNoise(rec->sasVoice, rec->field28);
    }
    idx = rec->sasVoice;
    v = &g_sndSsVoices[idx];
    if (v->note == 0xff) {
        SndSasSetPitch(idx, rec->pitch);
    } else {
        u16 factor = SndSsNoteToPitch(v->centerNoteU, (s16)((v->fine * 127) / 100), v->note,
                                      (s16)((v->centerNote * 127) / 100));
        pitch = (s32)((v->sampleRate * factor) / 44100 + v->field64);
        if (pitch > 0x4000) {
            pitch = 0x4000;
        }
        if (pitch < 1) {
            pitch = 1;
        }
        SndSasSetPitch(rec->sasVoice, pitch);
    }
    g_sndSsVoices[idx].field0c = g_sndSsVoices[idx].field64;
    SndSsPanToVolumes(rec->field34, rec->volume, rec->field38, rec->pan, &left, &right);
    SndSsPanToVolumes(rec->field3c, rec->volume2, rec->field40, rec->pan2, &effectLeft,
                      &effectRight);
    SndSasSetVolume(rec->sasVoice, left, right, effectLeft, effectRight);
    SndSasSetSimpleADSR(rec->sasVoice, rec->field44, rec->field48);
    SndSsStoreVoiceVolumes(rec->sasVoice, left, right, effectLeft, effectRight);
    rec->volL = rec->volume;
    rec->panL = rec->pan;
    rec->volR = rec->volume2;
    rec->panR = rec->pan2;
    return SndSasKeyOn(rec->sasVoice) < 0 ? -1 : 0;
}
