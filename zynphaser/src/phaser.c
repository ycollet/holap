/*
  ZynAddSubFX - a software synthesizer
 
  Phaser.C - Phaser effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for Zynphaser by Josep Andreu
  
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

#include <math.h>
#include <stdio.h>
#include "phaser.h"


#define PHASER_LFO_SHAPE 2
#define DENORMAL_GUARD 1e-18f
void
Phaser_Init(ZPhaser_t * s)
{
  Phaser_Cleanup (s);
}

/*
 * Effect output
 */
void
out (ZPhaser_t * s, float * smpsl, float * smpsr, unsigned long count)
{
  int i, j;
  float lfol, lfor, lgain, rgain, tmp;

  LADSPA_Data phase = *(s->Phase);
  LADSPA_Data depth = *(s->depth);
  LADSPA_Data panning = *(s->panning);
  LADSPA_Data lrcross = *(s->lrcross);
  LADSPA_Data fb = *(s->fb);
  LADSPA_Data *const outputl = s->efxoutl;
  LADSPA_Data *const outputr = s->efxoutr;
  LADSPA_Data Poutsub = *(s->Poutsub);
  LADSPA_Data Pstages = *(s->Pstages);

  if (s->Rstages != Pstages)
   {
     s->Rstages = Pstages;
     Phaser_Cleanup(s);
   }  

  effectlfoout (s, &lfol, &lfor, count);
  lgain = lfol;
  rgain = lfor;
  lgain =
    (expf (lgain * PHASER_LFO_SHAPE) - 1) / (expf (PHASER_LFO_SHAPE) - 1.0);
  rgain =
    (expf (rgain * PHASER_LFO_SHAPE) - 1) / (expf (PHASER_LFO_SHAPE) - 1.0);


  lgain = 1.0 - phase * (1.0 - depth) - (1.0 - phase) * lgain * depth;
  rgain = 1.0 - phase * (1.0 - depth) - (1.0 - phase) * rgain * depth;

  if (lgain > 1.0)
    lgain = 1.0;
  else if (lgain < 0.0)
    lgain = 0.0;
  if (rgain > 1.0)
    rgain = 1.0;
  else if (rgain < 0.0)
    rgain = 0.0;

  for (i = 0; i < count; i++)
    {
      float x = (float) i / count;
      float x1 = 1.0 - x;
      float gl = lgain * x + s->oldlgain * x1;
      float gr = rgain * x + s->oldrgain * x1;
      float inl = smpsl[i] * panning + s->fbl;
      float inr = smpsr[i] * (1.0 - panning) + s->fbr;

      //Left channel
      for (j = 0; j < Pstages * 2; j++)
	{			//Phasing routine
	  tmp = s->oldl[j]+DENORMAL_GUARD;
	  s->oldl[j] = gl * tmp + inl;
	  inl = tmp - gl * s->oldl[j];
	};
      //Right channel
      for (j = 0; j < Pstages * 2; j++)
	{			//Phasing routine
	  tmp = s->oldr[j]+DENORMAL_GUARD;
	  s->oldr[j] = (gr * tmp) + inr;
	  inr = tmp - (gr * s->oldr[j]);
	};
      //Left/Right crossing
      float l = inl;
      float r = inr;
      inl = l * (1.0 - lrcross) + r * lrcross;
      inr = r * (1.0 - lrcross) + l * lrcross;

      s->fbl = inl * fb;
      s->fbr = inr * fb;
      outputl[i] = inl;
      outputr[i] = inr;

    };

  s->oldlgain = lgain;
  s->oldrgain = rgain;

  if (Poutsub != 0)
    for (i = 0; i < count; i++)
      {
	outputl[i] *= -1.0;
	outputr[i] *= -1.0;
      };

};

/*
 * Cleanup the effect
 */
void
Phaser_Cleanup (ZPhaser_t * s)
{
  int i;

  s->fbl = 0.0;
  s->fbr = 0.0;
  s->oldlgain = 0.0;
  s->oldrgain = 0.0;

  for (i = 0; i < 24; i++)
    {
      s->oldl[i] = 0.0;
      s->oldr[i] = 0.0;
    };
};


