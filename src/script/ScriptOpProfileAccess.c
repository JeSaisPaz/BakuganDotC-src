// bdc 0x0880e41c ScriptOpProfileAccess
#include "bdc.h"

/* Script opcode handler to read or write the player save profile (`SaveGetProfile`). Operands: u16
   `op`, u32 `kind`, u32 `index`, then (op < 5: op != 1) a u32 value or (op == 1) an output ref
   (`ScriptReadRef`). `kind` 0 = counters
   (`SaveProfileGetCounter`/`SaveProfileSetCounter`/`SaveProfileAddCounter`, idx 0..2), `kind`
   1 = words (`SaveProfileGetWord`/`SaveProfileSetWord`, `SaveProfileAddWord` for op 4); with
   `index == 0` and op != 1 in kind 1 it instead touches the flag word (`SaveProfileSetFlags` for
   op 0 and 2, `SaveProfileClearFlags` for op 3, nothing otherwise); `kind` 2 reads
   `SaveProfileGetLanguage` into the ref. `op`: 0 = set, 1 = get into the ref, 4 = add (any other
   op also takes the get path). Always returns 0. */

int ScriptOpProfileAccess(Script *script)
{
  u32 op;
  s32 kind;
  u32 index;
  u32 value;
  u32 *ref;

  op = ScriptReadU16(script);
  kind = (s32)ScriptReadU32(script);
  index = ScriptReadU32(script);
  value = 0;
  ref = NULL;
  if (op < 5) {
    if (op == 1) {
      ref = ScriptReadRef(script, 2);
    } else {
      value = ScriptReadU32(script);
    }
  }

  if (kind == 0) {
    if (op == 4) {
      SaveProfileAddCounter(SaveGetProfile(), index, value);
    } else if (op == 0) {
      SaveProfileSetCounter(SaveGetProfile(), index, value);
    } else {
      *ref = SaveProfileGetCounter(SaveGetProfile(), index);
    }
  } else if (kind == 1) {
    if (index == 0 && op != 1) {
      if (op == 0 || op == 2) {
        SaveProfileSetFlags(SaveGetProfile(), value);
      } else if (op == 3) {
        SaveProfileClearFlags(SaveGetProfile(), value);
      }
    } else if (op == 4) {
      SaveProfileAddWord(SaveGetProfile(), index, value);
    } else if (op == 0) {
      SaveProfileSetWord(SaveGetProfile(), index, value);
    } else {
      *ref = SaveProfileGetWord(SaveGetProfile(), index);
    }
  } else if (kind == 2) {
    *ref = SaveProfileGetLanguage(SaveGetProfile());
  }
  return 0;
}
