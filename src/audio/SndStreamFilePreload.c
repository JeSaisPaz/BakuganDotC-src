// bdc 0x089c2b74 SndStreamFilePreload
#include "bdc.h"

/* Asks the data manager (CODataMng, `IoDataMngExists`) to read the `.at3` file of stream id `id`
   (`SndGetStreamFilePath`) into memory ahead of playback: `IoDataMngRequest(mgr, owner, path, 0,
   0, 1)` registers the load request (re-using an existing request for the same path) and
   `IoDataAddFlags(entry, 2)` sets bit 2 of the entry's state word (`entry + 0x30`) and clears its
   done byte (`+0x38`). Does nothing when the data manager does not exist yet. `owner`, the tag that
   the request is filed under, is the address of the player array `g_soundBgmPlayers`
   (`0x08ac5604`), except for the id stored in the one-entry table at `0x08aa11e8` (99999), which
   uses that cell's own address. */

void SndStreamFilePreload(s32 id)
{
  void *owner;
  int i;

  if (IoDataMngExists()) {
    owner = g_soundBgmPlayers;
    for (i = 0; i < 1; i++) {
      if ((s32)g_soundStreamSpecialIds[i] == id) {
        owner = g_soundStreamSpecialIds;
      }
    }
    IoDataAddFlags(IoDataMngRequest(IoGetDataMng(), owner, SndGetStreamFilePath(id), 0, false, true), 2);
  }
}
