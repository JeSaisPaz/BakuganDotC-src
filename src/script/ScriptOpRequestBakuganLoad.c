// bdc 0x0880f6b8 ScriptOpRequestBakuganLoad
#include "bdc.h"

/* Script opcode that queues the asset load of a Bakugan kind: reads u32 `kind` (0 = the player's
   current Bakugan, profile word 3, default 1) and creates a load request for it with
   `BtlLoadRequestCreate` (a Bakugan load request (`BtlLoadRequestCtor`) appended to `0x08aba7f0`,
   stepped by `BtlLoadRequestsUpdate`). The request object is not kept. Returns 0. */

int ScriptOpRequestBakuganLoad(Script *script)

{
  u32 kind;
  SaveProfile *self;
  
  kind = ScriptReadU32(script);
  if (kind == 0) {
    self = SaveGetProfile();
    kind = SaveProfileGetWord(self,3);
    if (kind == 0) {
      kind = 1;
    }
  }
  BtlLoadRequestCreate(kind);
  return 0;
}

