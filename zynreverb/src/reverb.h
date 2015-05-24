/*
  ZynAddSubFX - a software synthesizer
 
  Reverb.h - Reverberation effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for rakarrack by Josep Andreu

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License 
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#ifndef REVERB_H
#define REVERB_H

#include <ladspa.h>

#define RND (rand()/(RAND_MAX+1.0))



#define MAX_FILTER_STAGES 5

#define REV_COMBS 8
#define REV_APS 4


  typedef struct 
  {
    float c1, c2;
  } fstage; 
  

  typedef struct
  { 
  int order;			//the order of the filter (number of poles)
  int type;			//The type of the filter (LPF1,HPF1,LPF2,HPF2...)
  int stages;			//how many times the filter is applied (0->1,1->2,etc.)
  int needsinterpolation, firsttime;
  int abovenq;			//this is 1 if the frequency is above the nyquist
  int oldabovenq;		//if the last time was above nyquist (used to see if it needs interpolation)

  float outgain;
  float freq;		//Frequency given in Hz
  float q;			//Q factor (resonance or Q factor)
  float gain;		//the gain of the filter (if are shelf/peak) filters
  float c[3], d[3];		//coefficients
  float oldc[3], oldd[3];	//old coefficients(used only if some filter paremeters changes very fast, and it needs interpolation)
  float xd[3], yd[3];	//used if the filter is applied more times

   fstage x[6];
   fstage y[6];
   fstage oldx[6];
   fstage oldy[6];
   float ismp[2048];

} AnalogFilter;




  typedef struct
  {
  
  int idelaylen, rdelaylen;
  int idelayk;

  unsigned long SAMPLE_RATE;

  LADSPA_Data *efxoutl;
  LADSPA_Data *efxoutr;
  LADSPA_Data *inputl;
  LADSPA_Data *inputr;
  LADSPA_Data *outvolume;
  LADSPA_Data *dry;
  LADSPA_Data *panning; 
  LADSPA_Data *Ptime;
  LADSPA_Data *Pidelay;
  LADSPA_Data *idelayfb;
  LADSPA_Data *Proomsize;
  LADSPA_Data *Plpf;
  LADSPA_Data *Phpf;
  LADSPA_Data *Plohidamp;


  unsigned char Perbalance;
  //Low/HighFrequency Damping
  //unsigned char Plohidamp;	// 0..63 lpf,64=off,65..127=hpf(TODO)
  float pan, erbalance;
  //Parametrii 2  
  float lohifb, roomsize, rs;	//rs is used to "normalise" the volume according to the roomsize

  float Rtime;
  float Rlohidamp;
  float Rroomsize;
  float Rlpf;
  float Rhpf;

  float indatal[4096];
  float indatar[4096];
    
  int comblen[16];
  int aplen[8];

  // 352050   121084

  float comb[16][3521];
  int combk[16];
  float combfb[16];	//feedback-ul fiecarui filtru "comb"
  float lpcomb[16];	//pentru Filtrul LowPass
  float ap[8][1211];
  int apk[8];

  AnalogFilter lpf;
  AnalogFilter hpf;	//filters

  float inputbuf[2048];
  float idelay[80000];

}ZReverb_t;


  void Reverb_Init(ZReverb_t * s);
  void out (ZReverb_t * s, float * smps_l, float * smps_r, unsigned long count);
  void Reverb_Cleanup (ZReverb_t * s);
  void processmono (ZReverb_t * s, int ch, float * output, unsigned long count);
  void Reverb_settime(ZReverb_t * s, float RPtime);
  void Reverb_setlohidamp (ZReverb_t * s, float RPlohidamp);
  void Reverb_sethpf (ZReverb_t * s, float RPhpf);
  void Reverb_setlpf (ZReverb_t * s, float RPlpf);
  void Reverb_settype (ZReverb_t * s);
  void Reverb_setroomsize(ZReverb_t * s, float Rroomsize);

  void AnalogFilter_Init(ZReverb_t * s, AnalogFilter *filter, unsigned char Ftype, float Ffreq, float Fq, unsigned char Fstages);
  void filterout (ZReverb_t * s, AnalogFilter *filter, float * smp, unsigned long count);
  void setfreq (ZReverb_t * s, AnalogFilter *filter, float frequency);
  void setfreq_and_q (ZReverb_t * s, AnalogFilter *filter, float frequency, float q_);
  void setq (ZReverb_t * s, AnalogFilter *filter, float q_);
  void settype (ZReverb_t * s, AnalogFilter *filter, int type_);
  void setgain (ZReverb_t * s, AnalogFilter *filter, float dBgain);
  void setstages (ZReverb_t * s, AnalogFilter *filter, int stages_);
  void AnalogFilter_Cleanup (ZReverb_t * s, AnalogFilter *filter);

  float H(ZReverb_t * s, AnalogFilter *filter, float freq);	//Obtains the response for a given frequency

  void singlefilterout (ZReverb_t * s, AnalogFilter *filter, float *smp, fstage *x, fstage *y, float *c, float *d, unsigned long count);
   // Mirar fstage &x &y  //  ATENCION IMPORTANTE
  void computefiltercoefs (ZReverb_t * s, AnalogFilter *filter);







#endif
