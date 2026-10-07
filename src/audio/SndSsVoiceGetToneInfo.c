// bdc 0x08a234c8 SndSsVoiceGetToneInfo
#include "bdc.h"

/* Reads the pitch-related tone fields of voice `voice` from its record: the two pitch-bend ranges
   (`+0x50`, `+0x4f`), the played note (`+0x5d`), centre note / fine tune (`+0x4c..+0x4e`) and the
   sample rate (`+0x2c`). */

void SndSsVoiceGetToneInfo(s32 voice, u32 *bendUp, u32 *bendDown, u32 *note, s32 *centerNote, u32 *centerNoteU, s32 *fine, u32 *sampleRate)
{
  SndSsVoice *v = &g_sndSsVoices[voice];

  *bendUp = v->bendUp;
  *bendDown = v->bendDown;
  *note = v->note;
  *centerNote = v->centerNote;
  *centerNoteU = v->centerNoteU;
  *fine = v->fine;
  *sampleRate = v->sampleRate;
}
