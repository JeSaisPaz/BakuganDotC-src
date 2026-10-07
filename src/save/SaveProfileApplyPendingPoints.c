// bdc 0x089b1d78 SaveProfileApplyPendingPoints
#include "bdc.h"

/* Adds the pending-points word 0x2d of the player profile to the point counter `points` of the profile data,
   clamped to 0..9,999,999. */

void SaveProfileApplyPendingPoints(void)
{
  SaveProfile *self;
  SaveProfile *dst;
  int total;
  int result;

  total = SaveGetProfile()->data->points;
  self = SaveGetProfile();
  total = total + SaveProfileGetWord(self, 0x2d);
  dst = SaveGetProfile();
  result = 9999999;
  if ((total < 10000000) && (result = total, total < 0)) {
    result = 0;
  }
  dst->data->points = result;
}
