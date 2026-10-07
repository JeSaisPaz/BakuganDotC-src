// bdc 0x08a2113c SndSsBankGetToneParams
#include "bdc.h"

/* Copies the parameters of tone `tone` of bank `bankId` into the SndSsToneParams block at `out`: for
   a sampled tone the VAG entry (`SndSsPhdGetVag`) gives the absolute sample address (`pbd +
   offset`), size and rate; a tone whose VAG index is -1 is a noise tone (`isNoise = 1`, noise clock
   from the tone). Then the tone's two volume/pan pairs, ADSR words, fine tune, centre note/fine and
   pitch-bend ranges, key group and priority. Returns 0, or `0x80450005` when the tone or its VAG entry is missing. */

s32 SndSsBankGetToneParams(u32 bankId, u32 tone, u32 *out)
{
  SndSsToneParams *params;
  void ***entry;
  void **bank;
  s32 ret;
  SndSsPhdTone *toneEntry;
  SndSsPhdVag *vag;

  params = (SndSsToneParams *)out;
  entry = &g_sndSsBankTable[bankId];
  bank = *entry;
  ret = SndSsPhdGetTone(bank[0], tone, (void **)&toneEntry);
  if (ret < 0) {
    return (s32)0x80450005;
  }
  if (toneEntry->vagIndex == -1) {
    params->isNoise = 1;
    params->noiseClock = toneEntry->noiseClock;
  } else {
    bank = *entry;
    ret = SndSsPhdGetVag(bank[0], (u32)toneEntry->vagIndex, (void **)&vag);
    if (ret < 0) {
      return (s32)0x80450005;
    }
    params->isNoise = 0;
    bank = *entry;
    params->sampleSize = vag->size;
    params->sampleAddr = (u8 *)bank[1] + vag->offset;
    params->sampleRate = vag->sampleRate;
  }
  params->volume = toneEntry->volume;
  params->pan = toneEntry->pan;
  params->volume2 = toneEntry->volume2;
  params->pan2 = toneEntry->pan2;
  params->adsr1 = toneEntry->adsr1;
  params->adsr2 = toneEntry->adsr2;
  params->fineTune = toneEntry->fineTune;
  params->centerNote = toneEntry->centerNote;
  params->centerFine = toneEntry->centerFine;
  params->keyGroup = toneEntry->keyGroup;
  params->priority = toneEntry->priority;
  params->bendDown = toneEntry->bendDown;
  params->bendUp = toneEntry->bendUp;
  return 0;
}
