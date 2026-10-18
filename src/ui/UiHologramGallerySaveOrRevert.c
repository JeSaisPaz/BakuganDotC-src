// bdc 0x0891edec UiHologramGallerySaveOrRevert
#include "bdc.h"

/* Commits or reverts the point changes of the hologram gallery screen (`UiHologramGalleryCtor`,
   task 391; menu cursor `+0x77`, panel `+0x74`): with `commit`, when profile word 0x2d (the
   reserved points) is non-zero, subtracts it from the profile points `+0x464` (clamped to
   0..9999999) and leaves the word as is; without, calls `SaveProfileClearPlacedHolograms`
   (revert). */

void UiHologramGallerySaveOrRevert(UiHologramGallery *self, bool commit)
{
  SaveProfile *profile;
  int points;
  int result;

  if (commit) {
    if (SaveProfileGetWord(SaveGetProfile(), 0x2d) != 0) {
      profile = SaveGetProfile();
      points = profile->data->points - SaveProfileGetWord(SaveGetProfile(), 0x2d);
      result = 9999999;
      if (points < 10000000) {
        result = points;
        if (points < 0) {
          result = 0;
        }
      }
      profile->data->points = result;
    }
  } else {
    SaveProfileClearPlacedHolograms();
  }
}
