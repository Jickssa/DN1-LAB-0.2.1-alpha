#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elekloader/sdk_alpha_machine.h"

uint64_t dn1_lab_clip_count;
uint32_t dn1_lab_preclip_peak;
const char sdk_alpha_abi_v1[] = "host-test-sdk-alpha-1";
extern const struct elek_machine vesper_machine, mycel_machine, orbital_machine;
extern const struct elek_machine lithos_machine, photon_machine, anima_machine;
extern const uint32_t dn1p_vesper_state_bytes, dn1p_mycel_state_bytes, dn1p_orbital_state_bytes;
extern const uint32_t dn1p_lithos_state_bytes, dn1p_photon_state_bytes, dn1p_anima_state_bytes;

struct fixture { int32_t knobs[ELEK_UI_KNOB_MAX]; uint8_t state[ELEK_VOICE_STATE_MAX]; struct elek_voice v; };
static const struct elek_machine *const machines[]={
 &vesper_machine,&mycel_machine,&orbital_machine,&lithos_machine,&photon_machine,&anima_machine
};
static const uint32_t *const state_bytes[]={&dn1p_vesper_state_bytes,&dn1p_mycel_state_bytes,&dn1p_orbital_state_bytes,&dn1p_lithos_state_bytes,&dn1p_photon_state_bytes,&dn1p_anima_state_bytes};
static uint32_t rng_next(uint32_t *s){uint32_t x=*s;x^=x<<13;x^=x>>17;x^=x<<5;*s=x;return x;}
static int32_t abs32(int32_t x){return x<0?(x==INT32_MIN?INT32_MAX:-x):x;}
static void init(struct fixture *f,const struct elek_machine *m,int note,uint32_t lane)
{
 uint32_t k; memset(f,0,sizeof(*f));
 for(k=0;k<m->knob_count;k++)f->knobs[k]=m->knobs[k].def;
 f->v.knob=f->knobs;f->v.track=lane;f->v.start=1;f->v.pitch=note<<16;
 f->v.velocity=100;f->v.state=f->state;f->v.idle=0;f->v.cc=NULL;
}
static int metadata(const struct elek_machine *m)
{
 uint32_t k,p,e;
 if(!m||m->id<103||m->id>108||!m->name||!m->short_name||strlen(m->name)>16||strlen(m->short_name)>4||!m->render||m->knob_count!=15)return 0;
 for(k=0;k<m->knob_count;k++){
  const struct elek_knob *q=&m->knobs[k];
  if(!q->label||strlen(q->label)>5||q->max<=q->min||q->def<q->min||q->def>q->max)return 0;
 }
 for(p=0;p<2;p++){
  if(m->pages[p].label&&strlen(m->pages[p].label)>16)return 0;
  for(e=0;e<8;e++){const struct elek_knob *q=m->pages[p].encoder[e];if(q&&(q<m->knobs||q>=m->knobs+m->knob_count))return 0;}
 }
 return 1;
}
static int render(const struct elek_machine *m,struct fixture *f,int32_t *peak)
{
 int32_t out[ELEK_VOICE_FRAMES];uint32_t i;
 m->render(&f->v,out);f->v.start=0;
 for(i=0;i<ELEK_VOICE_FRAMES;i++){
  int32_t q=out[i];int32_t magnitude=abs32(q);
  if(magnitude>*peak)*peak=magnitude;
 }
 return 1;
}
static int sweep(const struct elek_machine *m)
{
 struct fixture f;int32_t peak=0;int note;uint32_t k;
 for(note=0;note<=127;note++){
  init(&f,m,note,0);
  for(k=0;k<m->knob_count;k++){
   int32_t d=m->knobs[k].def;
   f.knobs[k]=m->knobs[k].min;render(m,&f,&peak);
   init(&f,m,note,0);f.knobs[k]=d;render(m,&f,&peak);
   init(&f,m,note,0);f.knobs[k]=m->knobs[k].max;render(m,&f,&peak);
  }
 }
 if(peak<100000){fprintf(stderr,"%s too quiet: peak=%d\n",m->name,peak);return 0;}
 printf("PASS %-8s note 0..127 and each knob min/default/max; peak=%d (%.1f%% FS)\n",m->name,peak,100.0*(double)peak/2147483647.0);
 return 1;
}
static int default_signal(const struct elek_machine *m)
{
 struct fixture f;int32_t peak=0;uint32_t b;
 init(&f,m,60,0);
 for(b=0;b<128;b++)render(m,&f,&peak);
 if(peak<2000000){fprintf(stderr,"%s default patch too quiet: peak=%d\n",m->name,peak);return 0;}
 printf("PASS %-8s default patch 128 blocks; peak=%d (%.1f%% FS)\n",m->name,peak,100.0*(double)peak/2147483647.0);
 return 1;
}
static int extremes(const struct elek_machine *m)
{
 struct fixture f;int32_t peak=0;uint32_t k,b;int x;
 for(x=0;x<2;x++){
  init(&f,m,x?84:36,0);
  for(k=0;k<m->knob_count;k++)f.knobs[k]=x?m->knobs[k].max:m->knobs[k].min;
  for(b=0;b<512;b++)render(m,&f,&peak);
 }
 printf("PASS %-8s joint extremes sustained 512 blocks; peak=%d\n",m->name,peak);
 return 1;
}
static int randomized_lanes(const struct elek_machine *m)
{
 struct fixture f[8];int32_t peak=0;uint32_t seed=0x6a09e667u^(uint32_t)m->id,k;int b,l;
 for(l=0;l<8;l++)init(&f[l],m,36+l*6,(uint32_t)l);
 for(b=0;b<384;b++)for(l=0;l<8;l++){
  for(k=0;k<m->knob_count;k++){
   const struct elek_knob *q=&m->knobs[k];uint32_t span=(uint32_t)(q->max-q->min+1);
   f[l].knobs[k]=q->min+(int32_t)(rng_next(&seed)%span);
  }
  f[l].v.pitch=(int32_t)(rng_next(&seed)%128u)<<16;
  f[l].v.velocity=1+(int32_t)(rng_next(&seed)%127u);
  f[l].v.start=(b%47)==0;render(m,&f[l],&peak);
 }
 printf("PASS %-8s 8 independent lanes × 384 randomized blocks; peak=%d\n",m->name,peak);
 return 1;
}
static int order_independence(const struct elek_machine *m)
{
 struct fixture a[8],b[8];int32_t ao[8][32],bo[8][32],peak=0;uint32_t seed=0xbb67ae85u^(uint32_t)m->id,k;int block,l;
 for(l=0;l<8;l++){init(&a[l],m,36+l*7,(uint32_t)l);init(&b[l],m,36+l*7,(uint32_t)l);}
 for(block=0;block<128;block++){
  for(l=0;l<8;l++){
   for(k=0;k<m->knob_count;k++){
    const struct elek_knob *q=&m->knobs[k];uint32_t span=(uint32_t)(q->max-q->min+1);
    int32_t val=q->min+(int32_t)(rng_next(&seed)%span);a[l].knobs[k]=b[l].knobs[k]=val;
   }
   a[l].v.start=b[l].v.start=(block%41)==0;
  }
  for(l=0;l<8;l++){int32_t tmp[32];m->render(&a[l].v,tmp);memcpy(ao[l],tmp,sizeof(tmp));a[l].v.start=0;}
  for(l=7;l>=0;l--){int32_t tmp[32];m->render(&b[l].v,tmp);memcpy(bo[l],tmp,sizeof(tmp));b[l].v.start=0;}
  for(l=0;l<8;l++)if(memcmp(ao[l],bo[l],sizeof(ao[l]))){fprintf(stderr,"%s order-dependent lane %d block %d\n",m->name,l,block);return 0;}
 }
 (void)peak;printf("PASS %-8s render-order independence, 8 lanes × 128 blocks\n",m->name);return 1;
}
static int state_invariants(const struct elek_machine *m)
{
 struct fixture f;int32_t peak=0;uint32_t b,i;int64_t prior_total=-1;
 init(&f,m,60,2);
 if(strcmp(m->name,"VESPER")==0){
  struct probe {int8_t stencil[32];int32_t y1[2],y2[2];uint32_t read_q16,rng;uint8_t write_pos,initialized;} q;
  for(b=0;b<600;b++){
   render(m,&f,&peak);memcpy(&q,f.state,sizeof(q));
   if(q.write_pos>=32||q.read_q16>=0x200000u)return 0;
   for(i=0;i<2;i++)if(q.y1[i]<-22000||q.y1[i]>22000||q.y2[i]<-22000||q.y2[i]>22000)return 0;
  }
 } else if(strcmp(m->name,"ORBITAL")==0){
  struct probe {uint32_t phase[6],path_phase,rng;int32_t px,py;uint8_t initialized;} q;
  for(b=0;b<600;b++){
   render(m,&f,&peak);memcpy(&q,f.state,sizeof(q));
   if(q.px < -32767 || q.px > 32767 || q.py < -32767 || q.py > 32767)return 0;
  }
 } else if(strcmp(m->name,"MYCEL")==0){
  struct probe {uint32_t phase[4],rng;int32_t energy[4];uint8_t initialized;} ;
  int32_t dens=-1,chaos=-1;
  for(i=0;i<m->knob_count;i++){if(strcmp(m->knobs[i].label,"DENS")==0)dens=(int32_t)i;if(strcmp(m->knobs[i].label,"CHAOS")==0)chaos=(int32_t)i;}
  f.knobs[dens]=0;f.knobs[chaos]=0;
  for(b=0;b<1;b++)render(m,&f,&peak);
  for(b=0;b<300;b++){
   int64_t total=0;struct probe q2;
   render(m,&f,&peak);memcpy(&q2,f.state,sizeof(q2));
   for(i=0;i<4;i++){if(q2.energy[i]<0||q2.energy[i]>11000)return 0;total+=q2.energy[i];}
   if(prior_total>=0 && total>prior_total+2)return 0;
   prior_total=total;
  }
 } else if(strcmp(m->name,"LITHOS")==0){
  struct probe {uint32_t phase[6],rng;int32_t reservoir[3];uint8_t initialized;} q;
  for(i=0;i<m->knob_count;i++)if(strcmp(m->knobs[i].label,"DUST")==0)f.knobs[i]=0;
  render(m,&f,&peak);memcpy(&q,f.state,sizeof(q));
  prior_total=(int64_t)q.reservoir[0]+q.reservoir[1]+q.reservoir[2];
  for(b=0;b<300;b++){
   int64_t total;render(m,&f,&peak);memcpy(&q,f.state,sizeof(q));
   for(i=0;i<3;i++)if(q.reservoir[i]<0||q.reservoir[i]>11000)return 0;
   total=(int64_t)q.reservoir[0]+q.reservoir[1]+q.reservoir[2];
   if(total>prior_total+3)return 0;
   prior_total=total;
  }
 } else if(strcmp(m->name,"PHOTON")==0){
  struct probe {uint32_t phase[4],rng;int32_t energy[4];uint16_t clock;uint8_t initialized;} q;
  for(b=0;b<1000;b++){
   render(m,&f,&peak);memcpy(&q,f.state,sizeof(q));
   if(q.clock>64)return 0;
   for(i=0;i<4;i++)if(q.energy[i]<0||q.energy[i]>10000)return 0;
  }
 } else if(strcmp(m->name,"ANIMA")==0){
  struct probe {uint32_t phase[4],rng;int32_t activity[4],memory[4];uint8_t initialized;} q;
  for(b=0;b<600;b++){
   render(m,&f,&peak);memcpy(&q,f.state,sizeof(q));
   for(i=0;i<4;i++)if(q.activity[i]<0||q.activity[i]>8500||q.memory[i]<0||q.memory[i]>8500)return 0;
  }
 }
 printf("PASS %-8s internal state bounds / bounded energy invariant\n",m->name);return 1;
}
static int orbital_path_memory_test(void)
{
 const struct elek_machine *m=&orbital_machine;struct fixture a,b;int32_t peak=0,x[32],y[32];int32_t xi=-1,yi=-1,path=-1;
 uint32_t k;for(k=0;k<m->knob_count;k++){if(!strcmp(m->knobs[k].label,"X"))xi=(int32_t)k;if(!strcmp(m->knobs[k].label,"Y"))yi=(int32_t)k;if(!strcmp(m->knobs[k].label,"PATH"))path=(int32_t)k;}
 if(xi<0||yi<0||path<0)return 0;
 init(&a,m,60,0);init(&b,m,60,0);a.knobs[path]=b.knobs[path]=127;
 a.knobs[xi]=63;a.knobs[yi]=-64;b.knobs[xi]=-64;b.knobs[yi]=63;
 for(int block=0;block<96;block++){render(m,&a,&peak);render(m,&b,&peak);}
 a.knobs[xi]=b.knobs[xi]=0;a.knobs[yi]=b.knobs[yi]=0;
 m->render(&a.v,x);m->render(&b.v,y);
 if(!memcmp(x,y,sizeof(x))){fprintf(stderr,"ORBITAL path memory collapsed: distinct paths, same endpoint, identical output\n");return 0;}
 puts("PASS ORBITAL same endpoint retains distinct path-dependent timbre");return 1;
}
static int retrigger(const struct elek_machine *m)
{
 struct fixture a,b;int32_t x[32],y[32],peak=0;int j;
 init(&a,m,60,3);init(&b,m,60,3);
 for(j=0;j<96;j++)render(m,&a,&peak);
 a.v.start=1;b.v.start=1;
 m->render(&a.v,x);m->render(&b.v,y);
 if(memcmp(x,y,sizeof(x))){fprintf(stderr,"%s retrigger does not reset all state\n",m->name);return 0;}
 printf("PASS %-8s deterministic retrigger reset\n",m->name);return 1;
}
int main(void)
{
 unsigned i;uint64_t samples=0; (void)samples;
 for(i=0;i<sizeof(machines)/sizeof(machines[0]);i++){
  const struct elek_machine *m=machines[i];uint64_t before=dn1_lab_clip_count;int ok;
  if(!metadata(m)){fprintf(stderr,"%s metadata invalid\n",m->name);return 1;}
  if(*state_bytes[i]>ELEK_VOICE_STATE_MAX){fprintf(stderr,"%s voice state too large: %u bytes\n",m->name,*state_bytes[i]);return 1;}
  printf("INFO %-8s state=%u/256 bytes; ID=%d provisional\n",m->name,*state_bytes[i],m->id);
  ok=sweep(m)&&default_signal(m)&&extremes(m)&&randomized_lanes(m)&&order_independence(m)&&retrigger(m)&&state_invariants(m);
  if(!ok)return 1;
  if(dn1_lab_clip_count!=before){fprintf(stderr,"%s hard clamps=%llu preclip=%u\n",m->name,(unsigned long long)(dn1_lab_clip_count-before),dn1_lab_preclip_peak);return 1;}
  printf("PASS %-8s no hard clamps in prototype suite\n",m->name);
 }
 if(!orbital_path_memory_test())return 1;
 puts("PASS all six experimental host prototypes; host evidence only, not ColdFire/emulator/hardware certification");
 return 0;
}