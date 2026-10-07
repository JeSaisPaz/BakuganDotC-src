// bdc 0x089c2d14 SndStreamFileRelease
#include "bdc.h"

/* Drops the data-manager request made by `SndStreamFilePreload` once the file is loaded. Returns
   0 only while the file is still loading (the request exists but `SndStreamFileIsLoaded` is
   false), 1 otherwise, including when the data manager is missing. If something is filed under the
   player-array tag `g_soundBgmPlayers` and the file is loaded: with `id == -1` it detaches that
   tag from every request (`IoDataMngReleaseOwner`); with a real id it finds the request by path
   and detaches the id's owner tag (`IoDataMngRelease`; the tag is the cell `0x08aa11e8` for id
   99999, else the player array). A request with only that owner is flagged released
   (`IoDataAddFlags(entry, 0x10)`), one with other owners just loses this owner
   (`IoDataRemoveOwner`), i.e. the entries are reference counted by owner tag. */

bool SndStreamFileRelease(s32 id)
{
  IoDataMng *mgr;
  IoDataMng *findMgr;
  void *owner;
  void *req;
  int i;
  bool result = true;

  if (IoDataMngExists()) {
    owner = g_soundBgmPlayers;
    if (IoDataMngFindByOwner(IoGetDataMng(), owner, NULL) != NULL) {
      if (!SndStreamFileIsLoaded(id)) {
        if (IoDataMngFindByPath(IoGetDataMng(), SndGetStreamFilePath(id)) != NULL) {
          result = false;
        }
      } else if (id == -1) {
        IoDataMngReleaseOwner(IoGetDataMng(), g_soundBgmPlayers);
      } else {
        for (i = 0; i < 1; i++) {
          if ((s32)g_soundStreamSpecialIds[i] == id) {
            owner = g_soundStreamSpecialIds;
          }
        }
        mgr = IoGetDataMng();
        findMgr = IoGetDataMng();
        req = IoDataMngFindByPath(findMgr, SndGetStreamFilePath(id));
        IoDataMngRelease(mgr, owner, req);
      }
    }
  }
  return result;
}
