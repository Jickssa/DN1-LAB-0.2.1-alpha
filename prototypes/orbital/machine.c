/* SPDX-License-Identifier: GPL-2.0-or-later
 * ORBITAL — Gesture-Conditioned Harmonic Field. Host prototype; ID 105 provisional.
 */
#include <stdint.h>
#include "../common/proto_dsp.h"
enum { X, Y, PATH, TURN, FIELD, HARM, SPREAD, LOCK, FRIC, SLEW, FOLD, SUB, NOIS, VIB, MIX };
struct orbital_state { uint32_t phase[6],path_phase,rng; int32_t px,py; uint8_t initialized; };
typedef char orbital_state_must_fit[(sizeof(struct orbital_state)<=ELEK_VOICE_STATE_MAX)?1:-1];
const uint32_t dn1p_orbital_state_bytes=(uint32_t)sizeof(struct orbital_state);
static const struct elek_knob knobs[]={
 [X]={"X",.min=-64,.max=63,.def=0},[Y]={"Y",.min=-64,.max=63,.def=0},
 [PATH]={"PATH",.max=127,.def=46},[TURN]={"TURN",.max=127,.def=80},
 [FIELD]={"FIELD",.max=127,.def=72},[HARM]={"HARM",.max=127,.def=64},
 [SPREAD]={"SPRD",.max=127,.def=44},[LOCK]={"LOCK",.max=127,.def=32},
 [FRIC]={"FRIC",.max=127,.def=10},[SLEW]={"SLEW",.max=127,.def=72},
 [FOLD]={"FOLD",.max=127,.def=0},[SUB]={"SUB",.max=127,.def=0},
 [NOIS]={"NOIS",.max=127,.def=0},[VIB]={"VIB",.max=127,.def=8},
 [MIX]={"MIX",.max=127,.def=112}
};
static void orbital_render(const struct elek_voice *v,elek_sample *out)
{
 struct orbital_state *s=ELEK_STATE(struct orbital_state,v);
 static const int16_t base_semi[6]={0,12,19,24,31,36};
 int32_t x=v->knob[X],y=v->knob[Y],path=v->knob[PATH],turn=v->knob[TURN];
 int32_t field=v->knob[FIELD],harm=v->knob[HARM],spread=v->knob[SPREAD];
 int32_t lock=v->knob[LOCK],fric=v->knob[FRIC],slew=v->knob[SLEW];
 int32_t fold=v->knob[FOLD],sub=v->knob[SUB],noise=v->knob[NOIS];
 int32_t vib=v->knob[VIB],mix=v->knob[MIX],amps[6],target_x,target_y;
 uint32_t inc[6],i,n; int32_t px,py;
 if(v->start||!s->initialized){
  for(n=0;n<6;n++)s->phase[n]=0;
  s->path_phase=0;s->px=0;s->py=0;
  s->rng=0x4f1bbcdcu^(v->track*0x9e3779b9u)^((uint32_t)v->pitch*0x85ebca6bu);
  s->initialized=1;
 }
 target_x=x*500;target_y=y*500;
 for(n=0;n<6;n++){
  int32_t semi=base_semi[n]+(harm*(int32_t)n)/48+((int32_t)n-2)*spread/96;
  int32_t p=v->pitch+semi*65536;if(p<0)p=0;if(p>(127<<16))p=127<<16;
  inc[n]=elek_phase_inc(p);if(inc[n]>0x7fffffffu)inc[n]=0;
 }
 px=s->px;py=s->py;
 for(n=0;n<6;n++){
  int32_t ax=(px*(int32_t)(n+1)+py*(int32_t)(6-n))/6;
  int32_t ay=(py*(int32_t)(n+1)-px*(int32_t)(6-n))/6;
  int32_t modulation=(ax*(int32_t)(field+1)+ay*(int32_t)(127-field))/32768;
  amps[n]=dn1p_clamp_i32(2700+modulation,900,4700);
 }
 for(i=0;i<ELEK_VOICE_FRAMES;i++){
  int32_t lx=dn1_sin_q15(s->path_phase),ly=dn1_sin_q15(s->path_phase+0x40000000u);
  int32_t radius=path*150,tx=target_x+(lx*radius>>15),ty=target_y+(ly*radius>>15);
  int32_t alpha=8+(slew*40)/127,sample=0;
  if(turn<64)s->path_phase-=(uint32_t)(1000u+(uint32_t)path*500u);
  else s->path_phase+=(uint32_t)(1000u+(uint32_t)path*500u);
  s->px+=(tx-s->px)*alpha/512;s->py+=(ty-s->py)*alpha/512;
  if(lock){s->px+=(target_x-s->px)*lock/4096;s->py+=(target_y-s->py)*lock/4096;}
  if(fric){s->px+=(target_x-s->px)*fric/16384;s->py+=(target_y-s->py)*fric/16384;}
  for(n=0;n<6;n++){
   int32_t vib_amount=((int32_t)(inc[n]>>18)*vib)/127;
   int32_t delta=(vib_amount*lx)/32767;
   sample+=(dn1_sin_q15(s->phase[n])*amps[n])>>15;
   s->phase[n]+=(uint32_t)((int32_t)inc[n]+delta);
  }
  if(sub)sample+=(dn1_sin_q15(s->phase[0])*sub)>>10;
  if(noise)sample+=dn1p_abs_noise(&s->rng,noise/8+1);
  if(fold)sample=dn1_fold_q15((sample*(127+fold*3))/127);
  sample=(sample*3)/4;
  sample=dn1p_mix_q15(sample,sample/2,mix);
  out[i]=elek_from_q15(dn1p_clamp_q15(sample));
 }
 s->px=dn1p_clamp_i32(s->px,-32767,32767);s->py=dn1p_clamp_i32(s->py,-32767,32767);
}
const struct elek_machine orbital_machine={
 .id=105,.name="ORBITAL",.short_name="ORBT",ELEK_KNOBS(knobs),
 .pages={
  {"FIELD",{[ELEK_A]=&knobs[X],[ELEK_B]=&knobs[Y],[ELEK_C]=&knobs[PATH],[ELEK_D]=&knobs[TURN],[ELEK_E]=&knobs[FIELD],[ELEK_F]=&knobs[HARM],[ELEK_G]=&knobs[SPREAD],[ELEK_H]=&knobs[LOCK]}},
  {"MOTION",{[ELEK_A]=&knobs[FRIC],[ELEK_B]=&knobs[SLEW],[ELEK_C]=&knobs[FOLD],[ELEK_D]=&knobs[SUB],[ELEK_E]=&knobs[NOIS],[ELEK_F]=&knobs[VIB],[ELEK_G]=&knobs[MIX]}}
 },.render=orbital_render
};
ELEK_REGISTER_MACHINE(orbital_machine);
