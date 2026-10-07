// bdc 0x088a9474 ActorStageObjGetCategory
#include "bdc.h"

/* Maps a stage-object kind to its category: 0 big buildings/cranes, 1 buildings, 2 warehouses and
   other static props (default), 3 vehicles/containers/trees, 4 chimneys/plant tower, 5 landmarks
   (`ActorStageObjKindIsLandmark`) and kinds 0xcf..0xdf, 6 attribute landmarks, 7
   signs/billboards/street lights, 8 ruins, 9 crystals (0xa9..0xb1), 0xa screens, 0xb smoke, 0xc
   mines, 0xd wind generator. Stored at `+0x208` by `ActorStageObjBaseCtor`. */

int ActorStageObjGetCategory(int kind)

{
  int isLandmark;
  int category;
  
  category = 2;
  switch(kind) {
  case 1:
  case 3:
  case 4:
  case 5:
  case 0x4a:
  case 0x4c:
  case 0x4d:
  case 0x4f:
  case 0x50:
  case 0x53:
  case 0x54:
    category = 1;
    break;
  case 2:
  case 6:
    category = 0;
    break;
  case 7:
  case 8:
  case 9:
  case 10:
  case 0xb:
  case 0xc:
  case 0xd:
  case 0xe:
  case 0xf:
  case 0x2d:
  case 0x2e:
  case 0x2f:
  case 0x30:
  case 0x31:
  case 0x32:
  case 0x33:
  case 0x34:
  case 0x65:
  case 0x66:
  case 0x67:
  case 0x68:
  case 0x6a:
  case 0x6b:
  case 0x6c:
  case 0x6e:
  case 0x6f:
  case 0x70:
  case 0x71:
  case 0x72:
  case 0x7c:
  case 0x7d:
  case 0x7e:
  case 0x7f:
  case 0x80:
  case 0x81:
  case 0x82:
  case 0x85:
  case 0x87:
  case 0x8b:
  case 0x8c:
  case 0x99:
  case 0x9a:
  case 0xa7:
    category = 2;
    break;
  case 0x10:
  case 0x11:
  case 0x12:
    category = 4;
    break;
  case 0x13:
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
  case 0x19:
  case 0x1b:
  case 0x1e:
  case 0x1f:
  case 0x2c:
  case 0x38:
  case 0x39:
  case 0x3c:
  case 0x3d:
  case 0x3e:
  case 0x58:
  case 0x59:
  case 0x69:
  case 0x73:
  case 0x74:
  case 0x7b:
  case 0x83:
  case 0x84:
  case 0x86:
  case 0x88:
  case 0x89:
  case 0x8a:
  case 0xa8:
  case 0xb2:
    category = 3;
    break;
  case 0x1a:
  case 0x1c:
  case 0x1d:
  case 0x37:
  case 0x3b:
  case 0x56:
  case 0x57:
  case 0x5a:
  case 0x6d:
    category = 7;
    break;
  case 0x3f:
  case 0x4b:
  case 0x4e:
  case 0x51:
  case 0x52:
  case 0x55:
    category = 8;
    break;
  case 0xa9:
  case 0xaa:
  case 0xab:
  case 0xac:
  case 0xad:
  case 0xae:
  case 0xaf:
  case 0xb0:
  case 0xb1:
    category = 9;
    break;
  case 0xb3:
  case 0xb4:
    category = 0xc;
    break;
  case 0xb5:
    category = 0xd;
    break;
  case 0xb6:
  case 0xb7:
    category = 10;
    break;
  case 0xbb:
  case 0xbc:
  case 0xbd:
  case 0xbe:
  case 0xbf:
  case 0xc0:
  case 0xc1:
  case 0xc2:
  case 0xc3:
  case 0xc4:
  case 0xc5:
  case 0xc6:
  case 199:
  case 200:
  case 0xc9:
  case 0xca:
  case 0xcb:
  case 0xcc:
    category = 6;
    break;
  case 0xce:
    category = 0xb;
    break;
  case 0xcf:
  case 0xd0:
  case 0xd1:
  case 0xd2:
  case 0xd3:
  case 0xd4:
  case 0xd5:
  case 0xd6:
  case 0xd7:
  case 0xd8:
  case 0xd9:
  case 0xda:
  case 0xdb:
  case 0xdc:
  case 0xdd:
  case 0xde:
  case 0xdf:
    category = 5;
  }
  isLandmark = ActorStageObjKindIsLandmark(kind);
  if (isLandmark != 0) {
    category = 5;
  }
  return category;
}

