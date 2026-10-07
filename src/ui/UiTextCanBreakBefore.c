// bdc 0x088183ac UiTextCanBreakBefore
#include "bdc.h"

/* Word-wrap rule of the text printer: given the previous glyph `prev` and the current glyph `cur`,
   returns 1 when a line may break before `cur`, i.e. after a space-like glyph (`prev` 0 or -5)
   unless `cur` is one of the closing punctuation glyphs 0x206..0x20a; in French (language 2,
   `SaveProfileGetLanguage`) the glyphs 1, 0xc, 0xe, 0x1a, 0x1b and 0x1f are also kept on the
   line, as French typography puts a space before `!`, `?`, `:`, `;` and closing quotes. */

int UiTextCanBreakBefore(int prev, int cur)

{
  SaveProfile *self;
  s32 lang;
  int result;
  
  result = 0;
  if ((prev == -5) || (prev == 0)) {
    if ((cur != 0x206) && ((((cur != 0x207 && (cur != 0x208)) && (cur != 0x209)) && (cur != 0x20a)))
       ) {
      result = 1;
    }
    self = SaveGetProfile();
    lang = SaveProfileGetLanguage(self);
    if ((lang == 2) &&
       ((((cur == 1 || (cur == 0x1f)) || ((cur == 0x1a || ((cur == 0x1b || (cur == 0xe)))))) ||
        (cur == 0xc)))) {
      result = 0;
    }
  }
  return result;
}

