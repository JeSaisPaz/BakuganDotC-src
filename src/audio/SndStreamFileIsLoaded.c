// bdc 0x089c2c14 SndStreamFileIsLoaded
#include "bdc.h"

/* Polls whether a stream file requested with `SndStreamFilePreload` has finished loading in the
   data manager (CODataMng): for a real `id` it finds the request by path (`IoDataMngFindByPath(mgr,
   ``SndGetStreamFilePath``(id))`, ignoring requests already flagged released) and returns whether
   its done byte is set (`IoDataIsDone`); for `id == -1` it walks every request filed under the
   player-array tag `g_soundBgmPlayers` (`IoDataMngFindByOwner`) and returns 1 only when there
   is at least one and all are done. Returns 0 when the data manager does not exist or nothing
   matches. */

bool SndStreamFileIsLoaded(s32 id)
{
  IoData *req;
  bool result = false;

  if (IoDataMngExists()) {
    if (id == -1) {
      req = IoDataMngFindByOwner(IoGetDataMng(), g_soundBgmPlayers, NULL);
      if (req != NULL) {
        result = true;
        do {
          if (!IoDataIsDone(req)) {
            return false;
          }
          req = IoDataMngFindByOwner(IoGetDataMng(), g_soundBgmPlayers, req);
        } while (req != NULL);
      }
    } else {
      req = IoDataMngFindByPath(IoGetDataMng(), SndGetStreamFilePath(id));
      if (req != NULL && IoDataIsDone(req)) {
        result = true;
      }
    }
  }
  return result;
}
