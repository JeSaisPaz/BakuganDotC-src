// bdc 0x08a23648 SndSsVoiceApplyKeyOn
#include "bdc.h"

/* Programs a hardware voice from its key-on record `voice` (a `SndSsVoice`, SAS voice in
   `sasVoice`, key-on block in `params`): sets the sample address/size (looping) or, for a noise tone
   (`params.tone.isNoise`), the noise clock (`SndSasSetVoice` / `SndSasSetNoise`); sets the pitch
   (`SndSasSetPitch`) either to the record's raw `params.tone.pitch` (voice note 0xff, no offset
   or clamp) or from the voice table entry's note (`SndSsNoteToPitch` × sample rate / 44100 +
   pitch offset, clamped 1..0x4000) and records the
   applied offset in the table entry; computes the dry/effect volumes (`SndSsPanToVolumes`),
   applies them (`SndSasSetVolume`), the two ADSR words (`SndSasSetSimpleADSR`), stores the
   volumes (`SndSsStoreVoiceVolumes`), copies volume/pan to the applied copies and keys the voice
   on (`SndSasKeyOn`). Returns 0, or -1 when the key-on fails. Called by `SndSsVoiceQueueKeyOn`. */

s32 SndSsVoiceApplyKeyOn(SndSsVoice *voice)
{
    SndSsVoice *rec = voice;
    SndSsVoice *v;
    u32 idx;
    s32 pitch;
    u32 left;
    u32 right;
    u32 effectLeft;
    u32 effectRight;

    if (rec->params.tone.isNoise == 0) {
        SndSasSetVoice(rec->sasVoice, rec->params.tone.sampleAddr, rec->params.tone.sampleSize, 1);
    } else {
        SndSasSetNoise(rec->sasVoice, rec->params.tone.noiseClock);
    }
    idx = rec->sasVoice;
    v = &g_sndSsVoices[idx];
    if (v->params.note == 0xff) {
        SndSasSetPitch(idx, rec->params.tone.pitch);
    } else {
        u16 factor = SndSsNoteToPitch(v->params.tone.centerNote,
                                      (s16)((v->params.tone.centerFine * 127) / 100), v->params.note,
                                      (s16)((v->params.tone.fineTune * 127) / 100));
        pitch = (s32)((v->params.tone.sampleRate * factor) / 44100 + v->params.pitchOffset);
        if (pitch > 0x4000) {
            pitch = 0x4000;
        }
        if (pitch < 1) {
            pitch = 1;
        }
        SndSasSetPitch(rec->sasVoice, pitch);
    }
    g_sndSsVoices[idx].field0c = g_sndSsVoices[idx].params.pitchOffset;
    SndSsPanToVolumes(rec->params.tone.volume, rec->params.volume, rec->params.tone.pan,
                      rec->params.pan, &left, &right);
    SndSsPanToVolumes(rec->params.tone.volume2, rec->params.volume2, rec->params.tone.pan2,
                      rec->params.pan2, &effectLeft, &effectRight);
    SndSasSetVolume(rec->sasVoice, left, right, effectLeft, effectRight);
    SndSasSetSimpleADSR(rec->sasVoice, rec->params.tone.adsr1, rec->params.tone.adsr2);
    SndSsStoreVoiceVolumes(rec->sasVoice, left, right, effectLeft, effectRight);
    rec->volL = rec->params.volume;
    rec->panL = rec->params.pan;
    rec->volR = rec->params.volume2;
    rec->panR = rec->params.pan2;
    return SndSasKeyOn(rec->sasVoice) < 0 ? -1 : 0;
}
