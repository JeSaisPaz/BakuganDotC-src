// bdc 0x08a00930 ScriptOpMessageBox
#include "bdc.h"

/* Script opcode 0x41 (group 0x40): shows a message box. Operands u16 `cmd`, u16 `msgIndex`, inline
   string (the text, skipped with `ScriptSkipString`), u16 `mode` (sign-extended from 16 bits),
   u16 `value`. cmd 0/1 open the message box singleton (`UiMsgBoxEnsure`/`UiMsgBoxGet`) with
   the inline text (`UiMsgBoxPrintf`) or message `msgIndex` of `g_langStrings`
   (`UiMsgBoxPrintfUtf8`); when the text was set they store 50000.0f in `unk10`, open it with
   `mode` (`UiMsgBoxOpen`, flag 0), store `value` in `state` and return 0, else return 2. cmd 2
   requests closing (`UiMsgBoxRequestClose`) and returns 0; cmd 3 returns 0 once
   `UiMsgBoxIsFinished`, 2 until then. cmd 4..7 are the same four operations on the message
   window `g_uiMsgWindow` (`UiMsgWindowEnsure`, `UiMsgWindowPrintf`/`UiMsgWindowPrintfUtf8`,
   `depth` = 50000.0f and `mode` = `value`, then `UiMsgWindowOpen` with msgIndex -1;
   `UiMsgWindowRequestClose`; `UiMsgWindowIsClosed`). cmd >= 8 returns 2. */

int ScriptOpMessageBox(Script *script)

{
  u32 cmd;
  u32 msgIndex;
  char *text;
  u32 mode;
  u32 value;
  int openMode;
  bool ok;
  int ret;
  UiMsgBox *box;
  UiMsgWindow *win;

  ret = 2;
  cmd = ScriptReadU16(script);
  msgIndex = ScriptReadU16(script);
  /* ScriptSkipString leaves the string start in v0; read it before the skip. */
  text = (char *)script->operand;
  ScriptSkipString(script);
  mode = ScriptReadU16(script);
  value = ScriptReadU16(script);
  ok = false;
  switch (cmd) {
  case 0:
  case 1:
    if (!UiMsgBoxExists()) {
      UiMsgBoxEnsure();
    }
    if (cmd == 0) {
      if (UiMsgBoxPrintf(UiMsgBoxGet(), text)) {
        ok = true;
      }
    }
    else {
      if (UiMsgBoxPrintfUtf8(UiMsgBoxGet(), g_langStrings[msgIndex])) {
        ok = true;
      }
    }
    if (ok) {
      box = UiMsgBoxGet();
      box->unk10 = 50000.0f;
      openMode = (int)mode;
      if (openMode > 0x7fff) {
        openMode = openMode - 0x10000;
      }
      UiMsgBoxOpen(UiMsgBoxGet(), openMode, 0);
      box = UiMsgBoxGet();
      box->state = (int)value;
      ret = 0;
    }
    break;
  case 2:
    ret = 0;
    UiMsgBoxRequestClose(UiMsgBoxGet());
    break;
  case 3:
    ret = 2;
    if (UiMsgBoxIsFinished(UiMsgBoxGet())) {
      ret = 0;
    }
    break;
  case 4:
  case 5:
    if (!UiMsgWindowExists()) {
      UiMsgWindowEnsure();
    }
    if (cmd == 4) {
      if (UiMsgWindowPrintf(UiMsgWindowGet(), text) != 0) {
        ok = true;
      }
    }
    else {
      if (UiMsgWindowPrintfUtf8(UiMsgWindowGet(), g_langStrings[msgIndex]) != 0) {
        ok = true;
      }
    }
    if (ok) {
      win = UiMsgWindowGet();
      win->depth = 50000.0f;
      win = UiMsgWindowGet();
      win->mode = (int)value;
      openMode = (int)mode;
      if (openMode > 0x7fff) {
        openMode = openMode - 0x10000;
      }
      UiMsgWindowOpen(UiMsgWindowGet(), openMode, -1);
      ret = 0;
    }
    break;
  case 6:
    ret = 0;
    UiMsgWindowRequestClose(UiMsgWindowGet());
    break;
  case 7:
    ret = 2;
    if (UiMsgWindowIsClosed(UiMsgWindowGet())) {
      ret = 0;
    }
    break;
  }
  return ret;
}
