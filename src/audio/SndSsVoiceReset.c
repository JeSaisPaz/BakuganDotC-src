// bdc 0x08a23520 SndSsVoiceReset
#include "bdc.h"

/* Resets a voice record (0x68 bytes) to its defaults: no handle/key group/priority (-1), volumes 0,
   pans 0x40, channel 0xff, pitch offset 0. Leaves the SAS index, state and `+0x04`/`+0x18` alone. */

void SndSsVoiceReset(SndSsVoice *voice)
{
    voice->params.tone.isNoise = 1;
    voice->params.channel = 0xff;
    voice->params.pan2 = 0x40;
    voice->params.pitchOffset = 0;
    voice->volL = 0;
    voice->panL = 0x40;
    voice->volR = 0;
    voice->panR = 0x40;
    voice->field0c = 0;
    voice->params.handle = -1;
    voice->params.bankId = -1;
    voice->params.tone.sampleAddr = 0;
    voice->params.tone.sampleSize = 0;
    voice->params.tone.noiseClock = 0;
    voice->params.tone.sampleRate = -1;
    voice->params.tone.volume = 0;
    voice->params.tone.pan = 0x40;
    voice->params.tone.volume2 = 0;
    voice->params.tone.pan2 = 0x40;
    voice->params.tone.adsr1 = 0;
    voice->params.tone.adsr2 = 0;
    voice->params.tone.fineTune = 0;
    voice->params.tone.centerNote = 0;
    voice->params.tone.centerFine = 0;
    voice->params.tone.bendDown = 0;
    voice->params.tone.bendUp = 0;
    voice->params.tone.keyGroup = -1;
    voice->params.tone.priority = -1;
    voice->params.note = 0;
    voice->params.volume = 0;
    voice->params.pan = 0x40;
    voice->params.volume2 = 0;}
