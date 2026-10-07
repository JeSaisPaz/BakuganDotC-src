// bdc 0x0880a38c UiLanguageSelectStateFinish
#include "bdc.h"

/* Final state of the language-selection screen: registers the language's dialog font
   `dialog_moji_%s.rep` from the package as `dialog_moji` (`GfxBootTextureReplace`, found with
   `CorePackChainFind`) and sets the done byte `+0x58` so the update removes the task. */

void UiLanguageSelectStateFinish(CoreTask *task)

{
  UiLanguageSelect *self = (UiLanguageSelect *)task;
  const char *lang;
  void *data;
  u32 size;
  char name[128];

  if (self->pack != NULL) {
    lang = SaveGetLanguageDirName();
    sprintf(name, "dialog_moji_%s.rep", lang);
    data = CorePackChainFind(self->pack, name);
    size = CorePackChainFindSize(self->pack, name);
    GfxBootTextureReplace("dialog_moji", data, size);
  }
  self->done = 1;
}
