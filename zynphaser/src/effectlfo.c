/*
  ZynAddSubFX - a software synthesizer
 
  EffectLFO.C - Stereo LFO used by some effects
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for ZynAlienwah LADSPA plugin by Josep Andreu


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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "phaser.h"

void
EffectLFO_Init(ZPhaser_t * s)
{
 
  s->lfo.xl = 0.0;
  s->lfo.xr = 0.5;
  
   s->lfo.ampl1=1.3;
   s->lfo.ampl2=1.6;
   s->lfo.ampr1=1.1;
   s->lfo.ampr2=1.5;

}

/*
 * Compute the shape of the LFO
 */
float 
getlfoshape (ZPhaser_t * s, float x)
{
  
  LADSPA_Data PLFOtype = *(s->PLFOtype);
  
  float
    out;
  switch ((int) PLFOtype)
    {
    case 1:			//EffectLFO_TRIANGLE
      if ((x > 0.0) && (x < 0.25))
	out = 4.0 * x;
      else if ((x > 0.25) && (x < 0.75))
	out = 2 - 4 * x;
      else
	out = 4.0 * x - 4.0;
      break;
      //more to be added here; also ::updateparams() need to be updated (to allow more lfotypes)
    default:
      out = cosf (x * 2 * M_PI);	//EffectLFO_SINE
    };
  return (out);
};

/*
 * LFO output
 */
void
effectlfoout (ZPhaser_t * s, float * outl, float * outr, unsigned long count)
{
  float out;

  LADSPA_Data lfornd = *(s->lfornd);
  LADSPA_Data Pstereo = *(s->Pstereo);
  LADSPA_Data lfofreq = *(s->lfofreq);
  LADSPA_Data PLFOtype = *(s->PLFOtype);

  if (Pstereo != s->lfo.stereo)
     {
       s->lfo.stereo = Pstereo;
       s->lfo.xr=fmodf(s->lfo.xl+s->lfo.stereo+1.0,1.0);
     }


  s->lfo.lfofreq=(powf(2,lfofreq*10.0)-1.0)*0.03;
  s->lfo.incx = fabsf (s->lfo.lfofreq) * (float) count / (float) s->SAMPLE_RATE;
  if (s->lfo.incx > 0.49999999)  s->lfo.incx = 0.499999999;		//Limit the Frequency


  out = getlfoshape (s, s->lfo.xl);
  if ((PLFOtype == 0) || (PLFOtype == 1))
    out *= (s->lfo.ampl1 + s->lfo.xl * (s->lfo.ampl2 - s->lfo.ampl1));
  s->lfo.xl += s->lfo.incx;
  if (s->lfo.xl > 1.0)
    {
      s->lfo.xl -= 1.0;
      s->lfo.ampl1 = s->lfo.ampl2;
      s->lfo.ampl2 = (1.0 - lfornd) + lfornd * RND;
    };
  *outl = (out + 1.0) * 0.5;

  out = getlfoshape (s, s->lfo.xr);
  if ((PLFOtype == 0) || (PLFOtype == 1))
    out *= (s->lfo.ampr1 + s->lfo.xr * (s->lfo.ampr2 - s->lfo.ampr1));
  s->lfo.xr += s->lfo.incx;
  if (s->lfo.xr > 1.0)
    {
      s->lfo.xr -= 1.0;
      s->lfo.ampr1 = s->lfo.ampr2;
      s->lfo.ampr2 = (1.0 - lfornd) + lfornd * RND;
    };
  *outr = (out + 1.0) * 0.5;
};


