// bdc 0x088af9d4 ActorStageObjKindGetBreakPiece
#include "bdc.h"

/* Maps a stage-object kind to its collapse model for `ActorStageObjSpawnBreakModel`: 1
   `f0_break_building01` (kinds 1, 3..5, 0x22..0x24, 0x35, 0x3a, 0x40..0x42, 0x4a..0x55, …), 2
   `f0_break_warehouse01` (kinds 7..0x12, 0x2e..0x34, 0x56, 0x57, 0x65.., …), 3 `f0_break_crane01`
   (kind 6), 0 none. */

int ActorStageObjKindGetBreakPiece(int kind)
{
  if (kind < 0x25) {
    if (kind < 0x13) {
      if (kind <= 0) {
        return 0;
      }
      switch (kind) {
      case 1:
      case 3:
      case 4:
      case 5:
        return 1;
      case 6:
        return 3;
      case 7: case 8: case 9: case 10: case 0xb: case 0xc:
      case 0xd: case 0xe: case 0xf: case 0x10: case 0x11: case 0x12:
        return 2;
      default:
        return 0;
      }
    }
    if (kind >= 0x22) {
      return 1;
    }
    return 0;
  }
  if (kind < 0x4a) {
    if (kind < 0x2e) {
      return 0;
    }
    switch (kind) {
    case 0x2e: case 0x2f: case 0x30: case 0x31: case 0x32: case 0x33: case 0x34:
      return 2;
    case 0x35:
    case 0x3a:
    case 0x40:
    case 0x41:
    case 0x42:
      return 1;
    default:
      return 0;
    }
  }
  if (kind < 0x56) {
    return 1;
  }
  if (kind >= 0x90) {
    return 0;
  }
  switch (kind) {
  case 0x56:
  case 0x57:
  case 0x65:
  case 0x66:
  case 0x67:
  case 0x68:
  case 0x6c:
  case 0x6e:
  case 0x72:
  case 0x7d:
  case 0x7e:
  case 0x7f:
  case 0x80:
  case 0x81:
  case 0x82:
  case 0x85:
  case 0x87:
    return 2;
  case 0x5b:
  case 0x5c:
  case 0x5d:
  case 0x75:
  case 0x76:
  case 0x77:
  case 0x8d:
  case 0x8e:
  case 0x8f:
    return 1;
  default:
    return 0;
  }
}
