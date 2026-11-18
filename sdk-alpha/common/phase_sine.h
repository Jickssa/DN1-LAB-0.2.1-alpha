/* SPDX-License-Identifier: GPL-2.0-or-later
 * 256-interval quarter-wave Q15 sine table with linear interpolation.
 * Integer-only and bounded for the Digitone MKI ColdFire.
 */
#ifndef DN1_LAB_PHASE_SINE_H
#define DN1_LAB_PHASE_SINE_H
#include <stdint.h>
#ifdef DN1_LAB_METER_CLIPS
extern uint64_t dn1_lab_clip_count;
extern uint32_t dn1_lab_preclip_peak;
#define DN1_LAB_CLIP_HIT(value) do { \
    int32_t dn1_clip_value_ = (value); \
    uint32_t dn1_clip_magnitude_ = dn1_clip_value_ < 0 ? 0u - (uint32_t)dn1_clip_value_ : (uint32_t)dn1_clip_value_; \
    if (dn1_clip_magnitude_ > dn1_lab_preclip_peak) dn1_lab_preclip_peak = dn1_clip_magnitude_; \
    ++dn1_lab_clip_count; \
} while (0)
#else
#define DN1_LAB_CLIP_HIT(value) ((void)0)
#endif
static const int16_t dn1_quarter_sine[257] = {
    0, 201, 402, 603, 804, 1005, 1206, 1407, 1608, 1809, 2009, 2210,
    2410, 2611, 2811, 3012, 3212, 3412, 3612, 3811, 4011, 4210, 4410, 4609,
    4808, 5007, 5205, 5404, 5602, 5800, 5998, 6195, 6393, 6590, 6786, 6983,
    7179, 7375, 7571, 7767, 7962, 8157, 8351, 8545, 8739, 8933, 9126, 9319,
    9512, 9704, 9896, 10087, 10278, 10469, 10659, 10849, 11039, 11228, 11417, 11605,
    11793, 11980, 12167, 12353, 12539, 12725, 12910, 13094, 13279, 13462, 13645, 13828,
    14010, 14191, 14372, 14553, 14732, 14912, 15090, 15269, 15446, 15623, 15800, 15976,
    16151, 16325, 16499, 16673, 16846, 17018, 17189, 17360, 17530, 17700, 17869, 18037,
    18204, 18371, 18537, 18703, 18868, 19032, 19195, 19357, 19519, 19680, 19841, 20000,
    20159, 20317, 20475, 20631, 20787, 20942, 21096, 21250, 21403, 21554, 21705, 21856,
    22005, 22154, 22301, 22448, 22594, 22739, 22884, 23027, 23170, 23311, 23452, 23592,
    23731, 23870, 24007, 24143, 24279, 24413, 24547, 24680, 24811, 24942, 25072, 25201,
    25329, 25456, 25582, 25708, 25832, 25955, 26077, 26198, 26319, 26438, 26556, 26674,
    26790, 26905, 27019, 27133, 27245, 27356, 27466, 27575, 27683, 27790, 27896, 28001,
    28105, 28208, 28310, 28411, 28510, 28609, 28706, 28803, 28898, 28992, 29085, 29177,
    29268, 29358, 29447, 29534, 29621, 29706, 29791, 29874, 29956, 30037, 30117, 30195,
    30273, 30349, 30424, 30498, 30571, 30643, 30714, 30783, 30852, 30919, 30985, 31050,
    31113, 31176, 31237, 31297, 31356, 31414, 31470, 31526, 31580, 31633, 31685, 31736,
    31785, 31833, 31880, 31926, 31971, 32014, 32057, 32098, 32137, 32176, 32213, 32250,
    32285, 32318, 32351, 32382, 32412, 32441, 32469, 32495, 32521, 32545, 32567, 32589,
    32609, 32628, 32646, 32663, 32678, 32692, 32705, 32717, 32728, 32737, 32745, 32752,
    32757, 32761, 32765, 32766, 32767,
};
/* Overflow-safe convex interpolation for signed Q15 inputs and weight 0..65536.
 * Out-of-range weights are clamped to the upper endpoint. Products and sum fit int32_t. */
static inline int32_t dn1_lerp_q16(int32_t a, int32_t b, uint32_t weight)
{
    if (weight > 65536u) weight = 65536u;
    uint32_t wa = 65536u - weight;
    int32_t half_sum = a * (int32_t)(wa >> 1) + b * (int32_t)(weight >> 1);
    /* Both Q16 weights are odd together; fold their missing low bit back in. */
    if ((wa & 1u) && (weight & 1u))
        half_sum += (a + b) / 2;
    return half_sum / 32768;
}

/* Quantize toward zero to a power-of-two step. DSP callers use shifts 1..14;
 * the wide-shift guard defines the helper safely for every positive int32 shift. */
static inline int32_t dn1_quantize_pow2(int32_t x, int32_t shift)
{
    uint32_t magnitude;
    if (shift <= 0) return x;
    if (shift >= 31) return shift == 31 && x == INT32_MIN ? INT32_MIN : 0;
    if (x >= 0) return (int32_t)(((uint32_t)x >> shift) << shift);
    magnitude = 0u - (uint32_t)x;
    magnitude = (magnitude >> shift) << shift;
    if (magnitude == 0x80000000u) return INT32_MIN;
    return -(int32_t)magnitude;
}

/* Three bounded reflections used by the two phase/formant engines. The callers
 * prove their pre-fold input remains within +/-208122; this helper is not a
 * general-purpose saturator for arbitrary int32_t values. */
static inline int32_t dn1_fold_q15(int32_t x)
{
    if (x > 32767) x = 65534 - x;
    else if (x < -32767) x = -65534 - x;
    if (x > 32767) x = 65534 - x;
    else if (x < -32767) x = -65534 - x;
    if (x > 32767) x = 65534 - x;
    else if (x < -32767) x = -65534 - x;
    return x;
}

/* Signed vibrato depth in phase-increment units. Keep the intermediate signed:
 * mixing a uint32_t phase increment with a negative vibrato value would turn
 * the product unsigned before division and destroy the negative half-cycle.
 * Bounds for the Digitone pitch range: (phase_inc >> 15) <= 34200,
 * |vibrato_q15| <= 255, amount <= 127, so the largest product is < 1.11e9. */
static inline int32_t dn1_vibrato_delta_q16(uint32_t phase_inc, int32_t vibrato_q15, int32_t amount)
{
    int32_t base = (int32_t)(phase_inc >> 15);
    int32_t scaled = base * vibrato_q15;
    scaled = scaled * amount;
    return scaled / 127 / 16;
}

static inline int32_t dn1_sin_q15(uint32_t phase)
{
    uint32_t quadrant = phase >> 30;
    uint32_t offset = phase & 0x3fffffffu;
    uint32_t index;
    uint32_t fraction;
    int32_t a, b, value;
    if (quadrant & 1u)
        offset = 0x40000000u - offset;
    index = offset >> 22;
    if (index >= 256u) return (quadrant & 2u) ? -32767 : 32767;
    fraction = (offset >> 6) & 0xffffu;
    a = dn1_quarter_sine[index];
    b = dn1_quarter_sine[index + 1u];
    value = a + (((b - a) * (int32_t)fraction) >> 16);
    return (quadrant & 2u) ? -value : value;
}
#endif
