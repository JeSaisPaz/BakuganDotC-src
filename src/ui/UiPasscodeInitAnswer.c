// bdc 0x0893dad0 UiPasscodeInitAnswer
#include "bdc.h"

/* Sets up the puzzle of `UiPasscode` from its argument `arg`: copies the 8-byte
   entry of the table `g_uiPasscodeAnswers` (answer code, answer length, mode) into `+0x7e0`,
   clears the cursor (`+0x74..+0x76`), expands the code into answer digits `+0x7ec` (0xff-filled,
   `UiNumberToDigitsPadded`) and clears the entry count `+0x7fc`. */

void UiPasscodeInitAnswer(UiScreen *screen)

{
  UiPasscode *self = (UiPasscode *)screen;

  *(UiPasscodeAnswerEntry *)&self->answerCode = g_uiPasscodeAnswers[self->arg];
  self->onCommandRow = 0;
  memset(&self->focusSymbol, 0, 2);
  memset(self->answer, 0xff, 0xc);
  UiNumberToDigitsPadded(self->answer, self->answerCode, self->answerLen, 0xff);
  self->entryCount = 0;
}
