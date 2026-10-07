// bdc 0x08a1664c GmoModelComputeWorldMatricesBind
#include "bdc.h"

/* Walks the model's nodes (`self->nodes`, `self->nodeCount`) in order and writes each node's world
   matrix into `node->localMatrix` = parent matrix · local transform (column c of a matrix is
   elements 4c..4c+3). The parent matrix is the parent node's `localMatrix` (`parentIndex` >= 0)
   or, for a root, `self->rootMatrix` when model flag 1 (`flags28`) is set and
   `g_gmoIdentityMatrix` otherwise; its w lanes (elements 3/7/11/15) carry the inherited scale
   (1,1,1,1 for a root). The local transform is the `rotate` quaternion's rotation, columns scaled
   by `scale` × inherited scale, plus `translate` × inherited scale; node `flags42` 0x10 takes
   rotation and translation from `*matrix` (scale 1), 0x01 scales by `scale` only and passes the
   inherited scale on in the w lanes (with 0x20/0x40: unscaled, w = inherited × `scale`), 0x80
   rotates about the pivot `*block38`, 0x02 bakes the inherited scale into the parent's first
   three columns afterwards and zeroes their w lanes; parent flag 0x40 divides the inherited scale
   by the parent's `scale`. Node 0's rotation uses the `vdot` form, every later node's the scalar
   form, computed one node ahead. The `cache 0x1e` prefetches and `cache 0x18` line creation are
   hints with no C effect. */

void GmoModelComputeWorldMatricesBind(GmoModel *self)

{
  GmoNode *nodes;
  GmoNode *last;
  const float *root;
  const float *q;
  float rot[3][3]; /* rotation columns of the current node (M000) */
  float x, y, z, w;
  int count;
  int i;
  int c;
  int r;
  int k;

  if (self == NULL)
    return;
  nodes = self->nodes;
  q = nodes->rotate;
  x = q[0];
  y = q[1];
  z = q[2];
  w = q[3];
  rot[0][0] = w * w + z * -z + -y * y + -x * -x;
  rot[0][1] = w * z + z * w + -y * -x + -x * -y;
  rot[0][2] = w * -y + z * x + -y * w + -x * -z;
  rot[1][0] = -z * w + w * -z + x * y + -y * -x;
  rot[1][1] = -z * z + w * w + x * -x + -y * -y;
  rot[1][2] = -z * -y + w * x + x * w + -y * -z;
  rot[2][0] = y * w + -x * -z + w * y + -z * -x;
  rot[2][1] = y * z + -x * w + w * -x + -z * -y;
  rot[2][2] = y * -y + -x * x + w * w + -z * -z;

  if (self->flags28 & 1)
    root = self->rootMatrix;
  else
    root = (const float *)&g_gmoIdentityMatrix;
  count = self->nodeCount;
  if (count <= 0)
    return;
  last = nodes + count - 1;

  for (i = 0; i < count; i++) {
    GmoNode *node = &nodes[i];
    GmoNode *src = (last < node + 1) ? last : node + 1;
    GmoNode *parent = NULL;
    int flags;
    int parentFlags = 0;
    float P[4][4];
    float L[4][4];
    float t[3];
    float s[3];
    float inh[3];
    float adj[3];
    float colScale[3];
    float wrow[4];
    float nx, ny, nz, nw;

    if (node->parentIndex >= 0)
      parent = &nodes[node->parentIndex];
    flags = node->flags42;
    if (parent != NULL) {
      parentFlags = parent->flags42;
      for (c = 0; c < 4; c++)
        for (r = 0; r < 4; r++)
          P[c][r] = parent->localMatrix[c * 4 + r];
    } else {
      for (c = 0; c < 4; c++) {
        for (r = 0; r < 3; r++)
          P[c][r] = root[c * 4 + r];
        P[c][3] = 1.0f;
      }
    }
    for (k = 0; k < 3; k++) {
      t[k] = node->translate[k];
      s[k] = node->scale[k];
    }
    nx = src->rotate[0];
    ny = src->rotate[1];
    nz = src->rotate[2];
    nw = src->rotate[3];
    if (flags & 0x10) {
      const float *m = node->matrix;

      for (c = 0; c < 3; c++)
        for (r = 0; r < 3; r++)
          rot[c][r] = m[c * 4 + r];
      for (k = 0; k < 3; k++) {
        t[k] = m[12 + k];
        s[k] = 1.0f;
      }
    }

    /* inherited scale (C310) and, under parent flag 0x40, divided by the parent's scale (R103) */
    for (k = 0; k < 3; k++) {
      inh[k] = P[k][3];
      adj[k] = inh[k];
    }
    if (parentFlags & 0x40) {
      for (k = 0; k < 3; k++)
        adj[k] = adj[k] * (1.0f / parent->scale[k]);
    }
    for (k = 0; k < 3; k++)
      colScale[k] = adj[k] * s[k];
    wrow[0] = 0.0f;
    wrow[1] = 0.0f;
    wrow[2] = 0.0f;
    wrow[3] = 1.0f;
    if (flags & 0x01) {
      for (k = 0; k < 3; k++)
        wrow[k] = adj[k];
      if ((flags & 0x60) == 0) {
        for (k = 0; k < 3; k++)
          colScale[k] = s[k];
      } else {
        for (k = 0; k < 3; k++) {
          colScale[k] = 1.0f;
          wrow[k] = adj[k] * s[k];
        }
      }
    }
    for (k = 0; k < 3; k++)
      t[k] = t[k] * inh[k];
    for (c = 0; c < 3; c++)
      for (r = 0; r < 3; r++)
        rot[c][r] = rot[c][r] * colScale[c];

    if (flags & 0x80) {
      const float *pivot = node->block38;
      float p[3];
      float off[3];

      for (k = 0; k < 3; k++) {
        p[k] = pivot[k];
        off[k] = p[k] * inh[k];
      }
      if (flags & 0x01) {
        for (k = 0; k < 3; k++)
          p[k] = p[k] * wrow[k];
      }
      for (k = 0; k < 3; k++) {
        float rp = rot[0][k] * p[0] + rot[1][k] * p[1] + rot[2][k] * p[2];

        t[k] = (t[k] + off[k]) - rp;
      }
    }

    for (c = 0; c < 3; c++) {
      for (r = 0; r < 3; r++)
        L[c][r] = rot[c][r];
      L[c][3] = 0.0f;
    }
    for (r = 0; r < 3; r++)
      L[3][r] = t[r];
    L[3][3] = 1.0f;

    /* next node's rotation, scalar form, halved (doubled below) */
    {
      float xx = nx * nx;
      float yy = ny * ny;
      float zz = nz * nz;
      float xy = nx * ny;
      float yz = ny * nz;
      float zx = nz * nx;
      float wx = nw * nx;
      float wy = nw * ny;
      float wz = nw * nz;
      float hy = 0.5f - yy;
      float hz = 0.5f - zz;

      /* world = parent · local; the w lanes are replaced by wrow */
      for (c = 0; c < 4; c++) {
        for (r = 0; r < 3; r++)
          node->localMatrix[c * 4 + r] =
              L[c][0] * P[0][r] + L[c][1] * P[1][r] + L[c][2] * P[2][r] + L[c][3] * P[3][r];
        node->localMatrix[c * 4 + 3] = wrow[c];
      }

      /* flag 0x02 bakes the inherited scale into the parent; a root (no parent) would write
         through a NULL parent in the listing, so it is skipped here */
      if ((flags & 0x02) && parent != NULL) {
        for (c = 0; c < 3; c++) {
          for (r = 0; r < 3; r++)
            parent->localMatrix[c * 4 + r] = P[c][r] * inh[c];
          parent->localMatrix[c * 4 + 3] = 0.0f;
        }
      }

      rot[0][0] = hy - zz;
      rot[0][1] = xy + wz;
      rot[0][2] = zx - wy;
      rot[1][0] = xy - wz;
      rot[1][1] = hz - xx;
      rot[1][2] = yz + wx;
      rot[2][0] = zx + wy;
      rot[2][1] = yz - wx;
      rot[2][2] = hy - xx;
      for (c = 0; c < 3; c++)
        for (r = 0; r < 3; r++)
          rot[c][r] = rot[c][r] + rot[c][r];
    }
  }
}
