/*
  ZynAddSubFX - a software synthesizer
 
  Phaser.h - Phaser effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for zynphaser by Josep Andreu
  
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

#ifndef PHASER_H
#define PHASER_H
#include <ladspa.h>

#define RND (rand()/(RAND_MAX+1.0))


#define MAX_PHASER_STAGES 12


  typedef struct
  {
  float xl, xr;
  float incx;
  float ampl1, ampl2, ampr1, ampr2;
  float lfornd;
  float lfofreq;
  float stereo;
  } EffectLFO;


  typedef struct
  {
  unsigned long SAMPLE_RATE;
  LADSPA_Data *efxoutl;
  LADSPA_Data *efxoutr;
  LADSPA_Data *inputl;
  LADSPA_Data *inputr;
  LADSPA_Data *outvolume;	
  LADSPA_Data *dry; 
  LADSPA_Data *depth;
  LADSPA_Data *panning;
  LADSPA_Data *fb;
  LADSPA_Data *lrcross;
  LADSPA_Data *lfofreq;
  LADSPA_Data *lfornd;
  LADSPA_Data *PLFOtype;
  LADSPA_Data *Pstereo;
  LADSPA_Data *Phase;
  LADSPA_Data *Poutsub;
  LADSPA_Data *Pstages;
  float Rstages;
  float fbl, fbr;
  float oldl[24];
  float oldr[24];
  float oldlgain, oldrgain;
  float indatal[4096];
  float indatar[4096];
  EffectLFO lfo;		
  } ZPhaser_t;


  void Phaser_Init (ZPhaser_t * s);
  void out (ZPhaser_t * s, float * smpsl, float * smpsr, unsigned long count);
  void Phaser_Cleanup (ZPhaser_t * s);
  void EffectLFO_Init(ZPhaser_t * s);
  void effectlfoout (ZPhaser_t * s, float * outl, float * outr, unsigned long count);
  float getlfoshape (ZPhaser_t * s, float x);






#endif
