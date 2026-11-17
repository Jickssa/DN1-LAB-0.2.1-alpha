/* SPDX-License-Identifier: GPL-2.0-or-later
 * PHOTON — Event-Driven Phase Camera. Host prototype; ID 107 provisional.
 * Bounded sparse events inject a four-node phase/reflection network. No queue.
 */
#include <stdint.h>
#include "../common/proto_dsp.h"
enum { PULSE, REFLECT, COUPLE, DIFFUSE, DENS, DECAY, RATIO, SHIFT, SYNC, FEEDBACK, HARD, SOFT, VELOC, JITTER, MIX };
struct photon_state { uint32_t phase[4],rng; int32_t energy[4]; uint16_t clock; uint8_t initialized; };
typedef char photon_state_must_fit[(sizeof(struct photon_state)<=ELEK_VOICE_STATE_MAX)?1:-1];
const uint32_t dn1p_photon_state_bytes=(uint32_t)sizeof(struct photon_state);
static const struct elek_knob knobs[]={
 [PULSE]={"PULSE",.max=127,.def=92},[REFLECT]={"REFL",.max=127,.def=64},
 [COUPLE]={"COUP",.max=127,.def=46},[DIFFUSE]={"DIFF",.max=127,.def=12},
 [DENS]={"DENS",.max=127,.def=24},[DECAY]={"DECAY",.max=127,.def=88},
 [RATIO]={"RATIO",.max=127,.def=52},[SHIFT]={"SHIFT",.min=-64,.max=63,.def=0},
 [SYNC]={"SYNC",.max=127,.def=38},[FEEDBACK]={"FDBK",.max=127,.def=28},
 [HARD]={"HARD",.max=127,.def=40},[SOFT]={"SOFT",.max=127,.def=72},
 [VELOC]={"VELOC",.max=127,.def=100},[JITTER]={"JITR",.max=127,.def=8},
 [MIX]={"MIX",.max=127,.def=112}
};
static void photon_render(const struct elek_voice *v,elek_sample *out)
{
 struct photon_state *s=ELEK_STATE(struct photon_state,v);
 static const uint8_t ea[4]={0,1,2,3},eb[4]={1,2,3,0};
 int32_t pulse=v->knob[PULSE],reflect=v->knob[REFLECT],couple=v->knob[COUPLE];
 int32_t diffuse=v->knob[DIFFUSE],density=v->knob[DENS],decay=v->knob[DECAY];
 int32_t ratio=v->knob[RATIO],shift=v->knob[SHIFT],sync=v->knob[SYNC];
 int32_t feedback=v->knob[FEEDBACK],hard=v->knob[HARD],soft=v->knob[SOFT];
 int32_t veloc=v->knob[VELOC],jitter=v->knob[JITTER],mix=v->knob[MIX];
 uint32_t inc[4],i,n,period=64u-(uint32_t)density*45u/127u;
 if(period<5u)period=5u;
 if(v->start||!s->initialized){
  s->rng=0x13198a2eu^(v->track*0x9e3779b9u)^((uint32_t)v->pitch*0x85ebca6bu);
  for(n=0;n<4;n++){s->phase[n]=n*0x20000000u;s->energy[n]=0;}
  s->energy[0]=pulse*(16+v->velocity)/4;s->clock=(uint16_t)period;s->initialized=1;
 }
 for(n=0;n<4;n++){
  int32_t semis=(int32_t)(n*ratio/42)+((int32_t)n-1)*shift/24;
  int32_t p=v->pitch+semis*65536;if(p<0)p=0;if(p>(127<<16))p=127<<16;
  inc[n]=elek_phase_inc(p);if(inc[n]>0x7fffffffu)inc[n]=0;
 }
 for(i=0;i<ELEK_VOICE_FRAMES;i++){
  int32_t old[4],delta[4]={0,0,0,0},sum=0;
  if(s->clock>=period){
   int32_t amp=(pulse*(32+veloc))/12;
   if(jitter)amp+=dn1p_abs_noise(&s->rng,jitter/4+1);
   s->energy[0]=dn1p_clamp_i32(s->energy[0]+amp,0,12000);
   s->phase[0]+=((uint32_t)(dn1p_rng(&s->rng)&0xffffu)*reflect)/2u;
   s->clock=0;
  }
  s->clock++;
  for(n=0;n<4;n++)old[n]=s->energy[n];
  for(n=0;n<4;n++){
   uint32_t a=ea[n],b=eb[n];
   int32_t exchange=((old[b]-old[a])*(couple*120/127))>>15;
   delta[a]+=exchange;delta[b]-=exchange;
  }
  if(feedback){
   int32_t fb=(old[0]*feedback)/8192;
   delta[0]-=fb;delta[3]+=fb;
  }
  for(n=0;n<4;n++){
   int32_t e=old[n]+delta[n],loss=(127-decay)*12+soft*8+hard*3;
   if(loss>3200)loss=3200;
   e-=(e*loss)>>15;
   if(diffuse)e+=dn1p_abs_noise(&s->rng,diffuse/12+1);
   s->energy[n]=dn1p_clamp_i32(e,0,10000);
  }
  for(n=0;n<4;n++){
   uint32_t phase=s->phase[n]+(n*reflect*0x100000u);
   int32_t node=dn1_sin_q15(phase);
   sum+=(node*s->energy[n])>>(15+(n?1:0));
   s->phase[n]+=inc[n];
  }
  if(sync){
   int32_t phase_error=dn1p_phase_delta(s->phase[0],s->phase[2]);
   s->phase[2]+=(uint32_t)(((phase_error/256)*(sync/8))/128);
  }
  sum+=(hard*(s->energy[1]-s->energy[3]))/256;
  sum*=4;
  sum=dn1p_mix_q15(sum,sum/2,mix);
  out[i]=elek_from_q15(dn1p_clamp_q15(sum));
 }
}
const struct elek_machine photon_machine={
 .id=107,.name="PHOTON",.short_name="PHOT",ELEK_KNOBS(knobs),
 .pages={
  {"EVENTS",{[ELEK_A]=&knobs[PULSE],[ELEK_B]=&knobs[REFLECT],[ELEK_C]=&knobs[COUPLE],[ELEK_D]=&knobs[DIFFUSE],[ELEK_E]=&knobs[DENS],[ELEK_F]=&knobs[DECAY],[ELEK_G]=&knobs[RATIO],[ELEK_H]=&knobs[SHIFT]}},
  {"PHASE",{[ELEK_A]=&knobs[SYNC],[ELEK_B]=&knobs[FEEDBACK],[ELEK_C]=&knobs[HARD],[ELEK_D]=&knobs[SOFT],[ELEK_E]=&knobs[VELOC],[ELEK_F]=&knobs[JITTER],[ELEK_G]=&knobs[MIX]}}
 },.render=photon_render
};
ELEK_REGISTER_MACHINE(photon_machine);
