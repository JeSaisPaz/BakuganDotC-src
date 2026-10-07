// bdc 0x08a23520 SndSsVoiceReset
#include "bdc.h"

/* Resets a voice record (0x68 bytes) to its defaults: no handle/key group/priority (-1), volumes 0,
   pans 0x40, channel 0xff, pitch offset 0. Leaves the SAS index, state and `+0x04`/`+0x18` alone. */

void SndSsVoiceReset(SndSsVoice *voice)
{
    voice->field1c = 1;
    voice->channel = 0xff;
    voice->pan2 = 0x40;
    voice->field64 = 0;
    voice->volL = 0;
    voice->panL = 0x40;
    voice->volR = 0;
    voice->panR = 0x40;
    voice->field0c = 0;
    voice->handle = -1;
    voice->field14 = -1;
    voice->field20 = 0;
    voice->field24 = 0;
    voice->field28 = 0;
    voice->sampleRate = -1;
    voice->field34 = 0;
    voice->field38 = 0x40;
    voice->field3c = 0;
    voice->field40 = 0x40;
    voice->field44 = 0;
    voice->field48 = 0;
    voice->centerNote = 0;
    voice->centerNoteU = 0;
    voice->fine = 0;
    voice->bendDown = 0;
    voice->bendUp = 0;
    voice->keyGroup = -1;
    voice->priority = -1;
    voice->note = 0;
    voice->volume = 0;
    voice->pan = 0x40;
    voice->volume2 = 0;}
