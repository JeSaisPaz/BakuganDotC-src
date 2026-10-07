// bdc 0x08a0dd34 __umoddi3
#include "bdc.h"

/* libgcc `__umoddi3`: unsigned 64-bit remainder `a % b`. It is libgcc2's `__udivmoddi4` inlined with
   only the remainder kept: the `UDIV_NEEDS_NORMALIZATION` long division built on the C
   `udiv_qrnnd` of `longlong.h` (two 16-bit half-word steps per 32-bit digit), with the divisor
   normalised by `count_leading_zeros` through `__clz_tab` (`0x08aa5018`). A zero divisor traps
   (`1 / d0`, `break 7`). Used by `_vfprintf_r`/`_vfiprintf_r` to extract 64-bit digits. */

/* longlong.h `count_leading_zeros` (table version). */
#define COUNT_LEADING_ZEROS(count, x)                                              \
  do {                                                                             \
    u32 xr_ = (x);                                                                 \
    u32 a_ = xr_ < 0x10000u ? (xr_ < 0x100u ? 0 : 8) : (xr_ < 0x1000000u ? 16 : 24); \
    (count) = 32 - (__clz_tab[xr_ >> a_] + a_);                                    \
  } while (0)

/* longlong.h `__udiv_qrnnd_c`: (n1:n0) / d with d normalised and n1 < d. */
#define UDIV_QRNND(q, r, n1, n0, d)                                                \
  do {                                                                             \
    u32 dh_ = (d) >> 16;                                                           \
    u32 dl_ = (d) & 0xffff;                                                        \
    u32 r1_ = (n1) % dh_;                                                          \
    u32 q1_ = (n1) / dh_;                                                          \
    u32 m_ = q1_ * dl_;                                                            \
    u32 r0_;                                                                       \
    u32 q0_;                                                                       \
    r1_ = (r1_ << 16) | ((n0) >> 16);                                              \
    if (r1_ < m_) {                                                                \
      q1_--, r1_ += (d);                                                           \
      if (r1_ >= (d) && r1_ < m_)                                                  \
        q1_--, r1_ += (d);                                                         \
    }                                                                              \
    r1_ -= m_;                                                                     \
    r0_ = r1_ % dh_;                                                               \
    q0_ = r1_ / dh_;                                                               \
    m_ = q0_ * dl_;                                                                \
    r0_ = (r0_ << 16) | ((n0) & 0xffff);                                           \
    if (r0_ < m_) {                                                                \
      q0_--, r0_ += (d);                                                           \
      if (r0_ >= (d) && r0_ < m_)                                                  \
        q0_--, r0_ += (d);                                                         \
    }                                                                              \
    r0_ -= m_;                                                                     \
    (q) = (q1_ << 16) | q0_;                                                       \
    (r) = r0_;                                                                     \
  } while (0)

unsigned long long __umoddi3(unsigned long long a, unsigned long long b)

{
  u32 n0 = (u32)a;
  u32 n1 = (u32)(a >> 32);
  u32 d0 = (u32)b;
  u32 d1 = (u32)(b >> 32);
  u32 n2;
  u32 q0;
  u32 q1;
  u32 bm;
  u32 sh;

  if (d1 == 0) {
    if (d0 > n1) {
      /* 0q = nn / 0D */
      COUNT_LEADING_ZEROS(bm, d0);
      if (bm != 0) {
        d0 = d0 << bm;
        n1 = (n1 << bm) | (n0 >> (32 - bm));
        n0 = n0 << bm;
      }
      UDIV_QRNND(q0, n0, n1, n0, d0);
    } else {
      /* qq = NN / 0d */
      if (d0 == 0) {
        d0 = 1 / d0; /* divide by zero on purpose */
      }
      COUNT_LEADING_ZEROS(bm, d0);
      if (bm == 0) {
        n1 -= d0;
      } else {
        sh = 32 - bm;
        d0 = d0 << bm;
        n2 = n1 >> sh;
        n1 = (n1 << bm) | (n0 >> sh);
        n0 = n0 << bm;
        UDIV_QRNND(q1, n1, n2, n1, d0);
      }
      UDIV_QRNND(q0, n0, n1, n0, d0);
    }
    /* remainder is n0 >> bm */
    return (unsigned long long)(n0 >> bm);
  }

  if (d1 > n1) {
    /* 00 = nn / DD: the dividend is the remainder */
    return a;
  }

  /* 0q = NN / dd */
  COUNT_LEADING_ZEROS(bm, d1);
  if (bm == 0) {
    if (n1 > d1 || n0 >= d0) {
      u32 lo = n0 - d0;

      n1 = n1 - d1 - (n0 < lo);
      n0 = lo;
    }
    return ((unsigned long long)n1 << 32) | n0;
  } else {
    u32 m1;
    u32 m0;
    u64 prod;

    sh = 32 - bm;
    d1 = (d1 << bm) | (d0 >> sh);
    d0 = d0 << bm;
    n2 = n1 >> sh;
    n1 = (n1 << bm) | (n0 >> sh);
    n0 = n0 << bm;
    UDIV_QRNND(q0, n1, n2, n1, d1);
    prod = (u64)q0 * d0;
    m1 = (u32)(prod >> 32);
    m0 = (u32)prod;
    if (m1 > n1 || (m1 == n1 && m0 > n0)) {
      u32 lo = m0 - d0;

      m1 = m1 - d1 - (m0 < lo);
      m0 = lo;
    }
    /* remainder is (n1:n0 - m1:m0) >> bm */
    {
      u32 lo = n0 - m0;

      n1 = n1 - m1 - (n0 < lo);
      n0 = lo;
    }
    return ((unsigned long long)(n1 >> bm) << 32) | ((n1 << sh) | (n0 >> bm));
  }
}
