/* Host-test subset mirroring elekloader shared machine SDK alpha.
 * The production build must include the upstream header from elekloader. */
#ifndef DN1_LAB_HOST_SDK_ALPHA_MACHINE_H
#define DN1_LAB_HOST_SDK_ALPHA_MACHINE_H
#include <stdint.h>
#define ELEK_SAMPLE_RATE 48000
#define ELEK_VOICE_FRAMES 32
#define ELEK_VOICE_STATE_MAX 256
#define ELEK_UI_KNOB_MAX 15
#define ELEK_UI_PAGE_MAX 2
enum { ELEK_A, ELEK_B, ELEK_C, ELEK_D, ELEK_E, ELEK_F, ELEK_G, ELEK_H };
typedef int32_t elek_sample;
struct elek_glyph { uint16_t rows[16]; };
struct elek_knob { const char *label; int32_t min, max, def; const struct elek_glyph *glyph; };
struct elek_page { const char *label; const struct elek_knob *encoder[8]; };
struct elek_view { const char *label; uint32_t hidden; const struct elek_glyph *glyph; const char *value; };
struct elek_scratch { uint32_t used; char text[32][6]; };
struct elek_ui { const int32_t *knob; struct elek_view *view; int32_t page; uint32_t track; struct elek_scratch *scratch; };
struct elek_voice {
    const int32_t *knob; uint32_t track, start; int32_t pitch, velocity;
    void *state; uint32_t idle; const int32_t *cc;
};
struct elek_machine {
    int32_t id; const char *name, *short_name; const struct elek_knob *knobs;
    uint32_t knob_count; struct elek_page pages[ELEK_UI_PAGE_MAX];
    void (*present)(const struct elek_ui *ui);
    void (*render)(const struct elek_voice *, elek_sample *out);
    uint8_t play, prio;
};
#define ELEK_KNOBS(k) .knobs = (k), .knob_count = (uint32_t)(sizeof(k) / sizeof((k)[0]))
extern const char sdk_alpha_abi_v1[];
#define ELEK_REGISTER_MACHINE(machine) \
    static const char *const sdk_alpha_abi_##machine __attribute__((used)) = sdk_alpha_abi_v1; \
    const struct elek_machine *const sdk_alpha_registration_##machine = &(machine)
#define ELEK_STATE(type, v) \
    ((void)sizeof(char[sizeof(type) <= ELEK_VOICE_STATE_MAX ? 1 : -1]), (type *)(v)->state)
static inline elek_sample elek_from_q15(int32_t v)
{
    if (v < -32768) v = -32768;
    if (v > 32767) v = 32767;
    return (int32_t)((uint32_t)v << 16);
}
static inline uint32_t elek_phase_inc(int32_t pitch)
{
    static const uint32_t step[49] = {
        731558u,742199u,752994u,763947u,775059u,786332u,797770u,809374u,
        821146u,833090u,845208u,857501u,869974u,882628u,895466u,908491u,
        921705u,935112u,948713u,962513u,976513u,990717u,1005127u,1019747u,
        1034579u,1049628u,1064895u,1080384u,1096099u,1112042u,1128217u,
        1144627u,1161276u,1178167u,1195304u,1212690u,1230329u,1248225u,
        1266381u,1284800u,1303488u,1322448u,1341683u,1361199u,1380998u,
        1401085u,1421464u,1442140u,1463116u
    };
    if (pitch < 0) pitch = 0;
    if (pitch > (127 << 16)) pitch = 127 << 16;
    uint32_t q = (uint32_t)pitch >> 14, k = q % 48, f = (uint32_t)pitch & 0x3fffu;
    return (step[k] + (((step[k + 1] - step[k]) * f) >> 14)) << (q / 48);
}
#endif
