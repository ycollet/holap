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
#define MAX_EQ_BANDS 10


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
  LADSPA_Data *q;
  LADSPA_Data *gain1;
  LADSPA_Data *gain2;
  LADSPA_Data *gain3;
  LADSPA_Data *gain4;
  LADSPA_Data *gain5;
  LADSPA_Data *gain6;
  LADSPA_Data *gain7;
  LADSPA_Data *gain8;
  LADSPA_Data *gain9;
  LADSPA_Data *gain10;

  float Rq,Rgain1,Rgain2;
  float Rgain3,Rgain4,Rgain5;
  float Rgain6,Rgain7,Rgain8;
  float Rgain9,Rgain10;
  
  AnalogFilter b1_l;
  AnalogFilter b1_r;
  AnalogFilter b2_l;
  AnalogFilter b2_r;
  AnalogFilter b3_l;
  AnalogFilter b3_r;
  AnalogFilter b4_l;
  AnalogFilter b4_r;
  AnalogFilter b5_l;
  AnalogFilter b5_r;
  AnalogFilter b6_l;
  AnalogFilter b6_r;
  AnalogFilter b7_l;
  AnalogFilter b7_r;
  AnalogFilter b8_l;
  AnalogFilter b8_r;
  AnalogFilter b9_l;
  AnalogFilter b9_r;
  AnalogFilter b10_l;
  AnalogFilter b10_r;
  
  
  } ZEq10ban_t;
  
  


  void EQ_Init(ZEq10ban_t * s);
  void out (ZEq10ban_t * s, float * smpsl, float * smpr, unsigned long count);
  void changepar (ZEq10ban_t * s, int npar, unsigned char value);
  void EQ_Cleanup(ZEq10ban_t * s);

  void AnalogFilter_Init(ZEq10ban_t * s, AnalogFilter *filter, unsigned char Ftype, float Ffreq, float Fq, unsigned char Fstages);
  void filterout (ZEq10ban_t * s, AnalogFilter *filter, float * smp, unsigned long count);
  void setfreq (ZEq10ban_t * s, AnalogFilter *filter, float frequency);
  void setfreq_and_q (ZEq10ban_t * s, AnalogFilter *filter, float frequency, float q_);
  void setq (ZEq10ban_t * s, AnalogFilter *filter, float q_);
  void settype (ZEq10ban_t * s, AnalogFilter *filter, int type_);
  void setgain (ZEq10ban_t * s, AnalogFilter *filter, float dBgain);
  void setstages (ZEq10ban_t * s, AnalogFilter *filter, int stages_);
  void AnalogFilter_Cleanup (ZEq10ban_t * s, AnalogFilter *filter);
  float H(ZEq10ban_t * s, AnalogFilter *filter, float freq);	//Obtains the response for a given frequency
  void singlefilterout (ZEq10ban_t * s, AnalogFilter *filter, float *smp, fstage *x, fstage *y, float *c, float *d, unsigned long count);
  void computefiltercoefs (ZEq10ban_t * s, AnalogFilter *filter);





#endif
