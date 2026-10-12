// bdc 0x08a234c8 SndSsVoiceGetToneInfo
#include "bdc.h"

/* Reads the pitch-related tone fields of voice `voice` from its record: the two pitch-bend ranges
   (`+0x50`, `+0x4f`), the played note (`+0x5d`), centre note / fine tune (`+0x4c..+0x4e`) and the
   sample rate (`+0x2c`). */

void SndSsVoiceGetToneInfo(s32 voice, u32 *bendUp, u32 *bendDown, u32 *note, s32 *centerNote, u32 *centerNoteU, s32 *fine, u32 *sampleRate)
{
  SndSsVoice *v = &g_sndSsVoices[voice];

  *bendUp = v->params.tone.bendUp;
  *bendDown = v->params.tone.bendDown;
  *note = v->params.note;
  *centerNote = v->params.tone.fineTune;
  *centerNoteU = v->params.tone.centerNote;
  *fine = v->params.tone.centerFine;
  *sampleRate = v->params.tone.sampleRate;
}
