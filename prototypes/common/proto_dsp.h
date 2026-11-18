/* SPDX-License-Identifier: GPL-2.0-or-later
 * DN1-LAB host prototypes: bounded integer helpers shared by six experiments.
 * These are research sources, not yet qualified ColdFire modules.
 */
#ifndef DN1_LAB_PROTO_DSP_H
#define DN1_LAB_PROTO_DSP_H
#include <stdint.h>
#include "elekloader/sdk_alpha_machine.h"
#include "../../sdk-alpha/common/phase_sine.h"

static inline int32_t dn1p_clamp_i32(int32_t x, int32_t lo, int32_t hi)
{
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}
static inline int32_t dn1p_clamp_q15(int32_t x)
{
    if (x > 32767) { DN1_LAB_CLIP_HIT(x); return 32767; }
    if (x < -32767) { DN1_LAB_CLIP_HIT(x); return -32767; }
    return x;
}
static inline uint32_t dn1p_rng(uint32_t *state)
{
    uint32_t x = *state;
    if (x == 0u) x = 0x6d2b79f5u;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    *state = x; return x;
}
static inline uint32_t dn1p_phase_at(int32_t pitch_q16, int32_t semitones_q16)
{
    int32_t p = pitch_q16 + semitones_q16;
    if (p < 0) p = 0;
    if (p > (127 << 16)) p = 127 << 16;
    return elek_phase_inc(p);
}
static inline int32_t dn1p_abs_i32(int32_t x)
{
    return x < 0 ? (x == INT32_MIN ? INT32_MAX : -x) : x;
}
static inline int32_t dn1p_sign_i32(int32_t x) { return (x > 0) - (x < 0); }
/* Modular signed phase difference without an out-of-range uint32_t-to-int32_t cast. */
static inline int32_t dn1p_phase_delta(uint32_t a, uint32_t b)
{
    uint32_t d = a - b;
    if (d <= 0x7fffffffu) return (int32_t)d;
    return -(int32_t)(~d) - 1;
}
static inline int32_t dn1p_mix_q15(int32_t a, int32_t b, int32_t amount)
{
    amount = dn1p_clamp_i32(amount, 0, 127);
    return (a * (127 - amount) + b * amount) / 127;
}
static inline int32_t dn1p_lerp_q16(int32_t a, int32_t b, uint32_t f)
{
    if (f > 65535u) f = 65535u;
    return (a * (int32_t)(65536u - f) + b * (int32_t)f) / 65536;
}
static inline int32_t dn1p_abs_noise(uint32_t *rng, int32_t scale)
{
    return (((int32_t)(dn1p_rng(rng) >> 17) - 16384) * scale) / 16384;
}
static inline int32_t dn1p_soft_limit(int32_t x, int32_t limit)
{
    if (x > limit) return limit + (x - limit) / 4;
    if (x < -limit) return -limit + (x + limit) / 4;
    return x;
}
#endif
