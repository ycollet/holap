/*
  ZynAddSubFX - a software synthesizer
 
  EQ.h - EQ Effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for zyneq3par by Josep Andreu
  
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

#ifndef EQ_H
#define EQ_H

#include <ladspa.h>


#define RND (rand()/(RAND_MAX+1.0))

#define MAX_FILTER_STAGES 5
#define MAX_EQ_BANDS 3


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
  unsigned long SAMPLE_RATE;
  
  LADSPA_Data *efxoutl;
  LADSPA_Data *efxoutr;
  LADSPA_Data *inputl;
  LADSPA_Data *inputr;
  LADSPA_Data *outvolume;
  LADSPA_Data *lowfreq;
  LADSPA_Data *lowgain;
  LADSPA_Data *lowq;
  LADSPA_Data *midfreq;
  LADSPA_Data *midgain;
  LADSPA_Data *midq;
  LADSPA_Data *highfreq;
  LADSPA_Data *highgain;
  LADSPA_Data *highq;

  float Rlowfreq,Rlowgain,Rlowq;
  float Rmidfreq,Rmidgain,Rmidq;
  float Rhighfreq,Rhighgain,Rhighq;

  AnalogFilter Low_l;
  AnalogFilter Low_r;
  AnalogFilter Mid_l;
  AnalogFilter Mid_r;
  AnalogFilter High_l;
  AnalogFilter High_r;
  
  } ZEq3par_t;
  
  


  void EQ_Init(ZEq3par_t * s);
  void out (ZEq3par_t * s, float * smpsl, float * smpr, unsigned long count);
  void changepar (ZEq3par_t * s, int npar, unsigned char value);
  void EQ_Cleanup(ZEq3par_t * s);

  void AnalogFilter_Init(ZEq3par_t * s, AnalogFilter *filter, unsigned char Ftype, float Ffreq, float Fq, unsigned char Fstages);
  void filterout (ZEq3par_t * s, AnalogFilter *filter, float * smp, unsigned long count);
  void setfreq (ZEq3par_t * s, AnalogFilter *filter, float frequency);
  void setfreq_and_q (ZEq3par_t * s, AnalogFilter *filter, float frequency, float q_);
  void setq (ZEq3par_t * s, AnalogFilter *filter, float q_);
  void settype (ZEq3par_t * s, AnalogFilter *filter, int type_);
  void setgain (ZEq3par_t * s, AnalogFilter *filter, float dBgain);
  void setstages (ZEq3par_t * s, AnalogFilter *filter, int stages_);
  void AnalogFilter_Cleanup (ZEq3par_t * s, AnalogFilter *filter);
  float H(ZEq3par_t * s, AnalogFilter *filter, float freq);	//Obtains the response for a given frequency
  void singlefilterout (ZEq3par_t * s, AnalogFilter *filter, float *smp, fstage *x, fstage *y, float *c, float *d, unsigned long count);
  void computefiltercoefs (ZEq3par_t * s, AnalogFilter *filter);





#endif
