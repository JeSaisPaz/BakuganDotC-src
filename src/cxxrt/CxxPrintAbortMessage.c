// bdc 0x08a027b4 CxxPrintAbortMessage
#include "bdc.h"

/* Prints `"%s: %s\n"` on stderr with the generic header `CxxGetMessage``(1)` ("C++ runtime
   abort") followed by the text of `code`: `fprintf` on the stream at `_impure_ptr + 0xc`
   (`_reent._stderr`). Only caller: `CxxAbort`. */

void CxxPrintAbortMessage(int code)

{
  FILE *fp;
  const char *header;
  const char *text;

  fp = g_impurePtr->_stderr;
  header = CxxGetMessage(1);
  text = CxxGetMessage(code);
  fprintf(fp, "%s: %s\n", header, text);
  return;
}
