// bdc 0x088b21b0 ActorStageObjRecordMapAttrLandmarkKind
#include "bdc.h"

/* Maps a saved landmark-slot value to an attribute-landmark kind: values 0xe..0x1f → `value +
   0xad` = kinds 0xbb..0xcc (`FIRE_LV1`..`DARK_LV3`, `ActorStageObjAttrLandmarkCtor`); 0..0xd,
   0x20 and larger fall back to 0x22 (`LANDMARK01`), 2/3 to 0x5b. Used by
   `ActorStageObjRecordSpawn` for the player's landmark slots (profile bytes `+0x84..`). */

int ActorStageObjRecordMapAttrLandmarkKind(u32 value)

{
  u32 v;

  v = value & 0xff;
  if (v < 0x21) {
    switch (v) {
    case 0:
    case 0xd:
    case 0x20:
      return 0x22;
    case 1:
      return 0x22;
    case 2:
      return 0x5b;
    case 3:
      return 0x5b;
    case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 0xb: case 0xc:
      return 0x22;
    default:
      return (v + 0xad) & 0xff;
    }
  }
  return 0x22;
}
