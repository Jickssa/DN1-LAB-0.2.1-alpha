/* SPDX-License-Identifier: GPL-2.0-or-later
 * LITHOS — Spectral Erosion Engine. Host prototype; ID 106 provisional.
 */
#include <stdint.h>
#include "../common/proto_dsp.h"
enum { ERODE, DEPOSIT, FRACT, GRAIN, POROUS, SEAM, DUST, MASS, WEAR, REPAIR, TILT, SPREAD, HIT, DECAY, MIX };
struct lithos_state { uint32_t phase[6],rng; int32_t reservoir[3]; uint8_t initialized; };
typedef char lithos_state_must_fit[(sizeof(struct lithos_state)<=ELEK_VOICE_STATE_MAX)?1:-1];
const uint32_t dn1p_lithos_state_bytes=(uint32_t)sizeof(struct lithos_state);
static const struct elek_knob knobs[]={
 [ERODE]={"ERODE",.max=127,.def=50},[DEPOSIT]={"DEPOS",.max=127,.def=24},
 [FRACT]={"FRACT",.max=127,.def=38},[GRAIN]={"GRAIN",.max=127,.def=64},
 [POROUS]={"POROS",.max=127,.def=44},[SEAM]={"SEAM",.max=127,.def=24},
 [DUST]={"DUST",.max=127,.def=8},[MASS]={"MASS",.max=127,.def=72},
 [WEAR]={"WEAR",.max=127,.def=46},[REPAIR]={"REPR",.max=127,.def=16},
 [TILT]={"TILT",.min=-64,.max=63,.def=0},[SPREAD]={"SPRD",.max=127,.def=42},
 [HIT]={"HIT",.max=127,.def=90},[DECAY]={"DECAY",.max=127,.def=82},
 [MIX]={"MIX",.max=127,.def=110}
};
static void lithos_render(const struct elek_voice *v,elek_sample *out)
{
 struct lithos_state *s=ELEK_STATE(struct lithos_state,v);
 static const int16_t semis[6]={0,12,19,24,31,43};
 int32_t erode=v->knob[ERODE],deposit=v->knob[DEPOSIT],fract=v->knob[FRACT];
 int32_t grain=v->knob[GRAIN],porous=v->knob[POROUS],seam=v->knob[SEAM];
 int32_t dust=v->knob[DUST],mass=v->knob[MASS],wear=v->knob[WEAR],repair=v->knob[REPAIR];
 int32_t tilt=v->knob[TILT],spread=v->knob[SPREAD],hit=v->knob[HIT];
 int32_t decay=v->knob[DECAY],mix=v->knob[MIX],amp[6];
 uint32_t inc[6],i,n;
 if(v->start||!s->initialized){
  s->rng=0x243f6a88u^(v->track*0x9e3779b9u)^((uint32_t)v->pitch*0x85ebca6bu);
  for(n=0;n<6;n++)s->phase[n]=0;
  s->reservoir[0]=3800;s->reservoir[1]=900;s->reservoir[2]=250;s->initialized=1;
 }
 for(n=0;n<6;n++){
  int32_t p=v->pitch+(semis[n]+tilt/8+((int32_t)n-2)*spread/64)*65536;
  if(p<0)p=0;if(p>(127<<16))p=127<<16;
  inc[n]=elek_phase_inc(p);if(inc[n]>0x7fffffffu)inc[n]=0;
  amp[n]=900+(mass*(int32_t)(6-n)*14)/127;
 }
 for(i=0;i<ELEK_VOICE_FRAMES;i++){
  int32_t smooth=s->reservoir[0],gran=s->reservoir[1],fractured=s->reservoir[2];
  int32_t flow_sg=(smooth*erode)/4096,flow_gs=(gran*deposit)/4096;
  int32_t flow_gf=(gran*fract)/4096,flow_sf=(smooth*fract*porous)/(127*8192);
  int32_t flow_fs=(fractured*repair)/4096;
  int32_t loss0=(smooth*((127-decay)+wear+mass/4))/24000;
  int32_t loss1=(gran*((127-decay)+grain/2+wear))/22000;
  int32_t loss2=(fractured*((127-decay)+wear/2))/26000;
  smooth+=-flow_sg+flow_gs-flow_sf+flow_fs-loss0;
  gran+=flow_sg-flow_gs-flow_gf-loss1;
  fractured+=flow_gf+flow_sf-flow_fs-loss2;
  if(i==0&&v->start)smooth+=hit*28;
  if(dust)fractured+=dn1p_abs_noise(&s->rng,dust/10+1);
  s->reservoir[0]=dn1p_clamp_i32(smooth,0,11000);
  s->reservoir[1]=dn1p_clamp_i32(gran,0,9000);
  s->reservoir[2]=dn1p_clamp_i32(fractured,0,9000);
  {
   int32_t sample=0;
   for(n=0;n<6;n++){
    int32_t r=s->reservoir[n/2],a=(r*amp[n])/18000;
    sample+=(dn1_sin_q15(s->phase[n])*a)>>15;
    s->phase[n]+=inc[n];
   }
   if(seam)sample+=((s->reservoir[2]-s->reservoir[0])*seam)/256;
   if(grain)sample+=dn1p_abs_noise(&s->rng,grain/12+1);
   sample=(sample*21)/2;
   sample=dn1p_mix_q15(sample,sample/2,mix);
   out[i]=elek_from_q15(dn1p_clamp_q15(sample));
  }
 }
}
const struct elek_machine lithos_machine={
 .id=106,.name="LITHOS",.short_name="LITH",ELEK_KNOBS(knobs),
 .pages={
  {"EROSION",{[ELEK_A]=&knobs[ERODE],[ELEK_B]=&knobs[DEPOSIT],[ELEK_C]=&knobs[FRACT],[ELEK_D]=&knobs[GRAIN],[ELEK_E]=&knobs[POROUS],[ELEK_F]=&knobs[SEAM],[ELEK_G]=&knobs[DUST],[ELEK_H]=&knobs[MASS]}},
  {"GRAIN",{[ELEK_A]=&knobs[WEAR],[ELEK_B]=&knobs[REPAIR],[ELEK_C]=&knobs[TILT],[ELEK_D]=&knobs[SPREAD],[ELEK_E]=&knobs[HIT],[ELEK_F]=&knobs[DECAY],[ELEK_G]=&knobs[MIX]}}
 },.render=lithos_render
};
ELEK_REGISTER_MACHINE(lithos_machine);
