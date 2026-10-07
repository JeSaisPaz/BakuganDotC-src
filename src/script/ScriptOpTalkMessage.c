// bdc 0x0880fefc ScriptOpTalkMessage
#include "bdc.h"

/* Script opcode handler for the in-game talk/dialogue window task (core id `0x6e`, `UiTalk*`;
   exists/get = `0x0882c0fc`/`0x0882c11c`). Operands: nine u16 values (`cmd`, a..h), each
   sign-extended. `cmd` 0: `UiTalkShowMessage``(task, a, b, c, d, e, f, g, 0, h)`; 2: same with
   1 in place of the 0; 3: same as 0 with `d = -1`; 4: `UiTalkRequestClose``(task, b)`; 1 and
   values outside 0..4 do nothing. Returns 0. Does nothing if the talk task does not exist. */

int ScriptOpTalkMessage(Script *script)

{
  s16 cmd;
  s16 a;
  s16 b;
  s16 c;
  s16 d;
  s16 e;
  s16 f;
  s16 g;
  s16 h;

  cmd = (s16)ScriptReadU16(script);
  a = (s16)ScriptReadU16(script);
  b = (s16)ScriptReadU16(script);
  c = (s16)ScriptReadU16(script);
  d = (s16)ScriptReadU16(script);
  e = (s16)ScriptReadU16(script);
  f = (s16)ScriptReadU16(script);
  g = (s16)ScriptReadU16(script);
  h = (s16)ScriptReadU16(script);
  if (UiTalkTaskExists() != 0 && (u32)(s32)cmd < 5) {
    switch (cmd) {
    case 1:
      break;
    case 2:
      UiTalkShowMessage(UiGetTalkTask(), a, b, c, d, e, f, g, 1, h);
      break;
    case 3:
      UiTalkShowMessage(UiGetTalkTask(), a, b, c, -1, e, f, g, 0, h);
      break;
    case 4:
      UiTalkRequestClose(UiGetTalkTask(), b);
      break;
    default:
      UiTalkShowMessage(UiGetTalkTask(), a, b, c, d, e, f, g, 0, h);
      break;
    }
  }
  return 0;
}
