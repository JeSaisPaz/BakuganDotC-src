// bdc 0x088cc368 UiTalkBalloonSetSpeaker
#include "bdc.h"

/* Sets the speaker of the talk balloon (`UiTalkBalloonCtor`): loads `"mes_CharacterName_%s.bin"`
   for the current language (`SaveGetLanguageName`) from the pack chain, relocates it
   (`UiMesTableRelocate`) and stores the speaker id `speaker` and name text `speakerName` (-1 / NULL
   when out of range). */

void UiTalkBalloonSetSpeaker(UiTalkBalloon *self, s16 speaker)
{
  const char *language;
  u32 *table;
  u32 count;
  char name[64];

  language = SaveGetLanguageName();
  sprintf(name, "mes_CharacterName_%s.bin", language);
  table = CorePackChainFind(g_ioLzsPackages, name);
  count = UiMesTableRelocate(table);
  self->speakerName = NULL;
  if ((int)speaker < (int)count) {
    self->speaker = speaker;
    self->speakerName = (char *)PspPtr(table[speaker]);
  } else {
    self->speaker = -1;
  }
}
