/*
  ZynAddSubFX - a software synthesizer
 
  Chorus.C - Chorus and Flange effects
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for ZynChorus LADSPA plugin by Josep Andreu

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
#include "chorus.h"

void
Chorus_Init(ZChorus_t * s)
{
  s->dlk = 0;
  s->drk = 0;
  s->maxdelay = (int) (MAX_CHORUS_DELAY / 1000.0 * s->SAMPLE_RATE);

//  Ppreset = 0;
//  setpreset (Ppreset);

Chorus_cleanup(s);

};



/*
 * get the delay value in samples; xlfo is the current lfo value
 */
float
getdelay (ZChorus_t * s, float xlfo)
{
  float
    result;

     LADSPA_Data delay = *(s->delay);
     LADSPA_Data depth = *(s->depth);

     s->Rdelay = (powf(10.0,delay*2.0)-1.0)/1000.0;
     s->Rdepth = (powf(8.0,depth*2.0)-1.0)/1000.0;

      result = (s->Rdelay + xlfo * s->Rdepth) * s->SAMPLE_RATE;

  //check if it is too big delay(caused bu errornous setdelay() and setdepth()    
  if ((result + 0.5) >= s->maxdelay)
    {
      fprintf (stderr, "%s",
	       "WARNING: Chorus.C::getdelay(..) too big delay (see setdelay and setdepth funcs.)\n");
      printf ("%f %d\n", result, s->maxdelay);
      result = s->maxdelay - 1.0;
    };
  return (result);
};

/*
 * Apply the effect
 */
void
out(ZChorus_t * s, float *smpsl, float *smpsr, unsigned long count)
{
  int i;
  LADSPA_Data *const outputl = s->efxoutl;
  LADSPA_Data *const outputr = s->efxoutr;
  LADSPA_Data subtract = *(s->Poutsub);
  LADSPA_Data lrcross = *(s->lrcross);    
  LADSPA_Data fb = *(s->fb);
  LADSPA_Data panning = *(s->panning);
  LADSPA_Data vol = *(s->outvolume);

  s->dl1 = s->dl2;
  s->dr1 = s->dr2;

  effectlfoout (s, &s->lfol, &s->lfor, count);

  s->dl2 = getdelay (s, s->lfol);
  s->dr2 = getdelay (s, s->lfor);

  for (i = 0; i < count; i++)
    {
      float inl = smpsl[i];
      float inr = smpsr[i];
      //LRcross
      float l = inl;
      float r = inr;
      inl = l * (1.0 - lrcross) + r * lrcross;
      inr = r * (1.0 - lrcross) + l * lrcross;

      //Left channel

      //compute the delay in samples using linear interpolation between the lfo delays
      s->mdel = (s->dl1 * (count - i) + s->dl2 * i) / count;
      if (++s->dlk >= s->maxdelay)
	s->dlk = 0;
      float tmp = s->dlk - s->mdel + s->maxdelay * 2.0;	//where should I get the sample from

      F2I (tmp, s->dlhi);
      s->dlhi %= s->maxdelay;

      s->dlhi2 = (s->dlhi - 1 + s->maxdelay) % s->maxdelay;
      s->dllo = 1.0 - fmodf (tmp, 1.0);
      outputl[i] = s->delayl[s->dlhi2] * s->dllo + s->delayl[s->dlhi] * (1.0 - s->dllo);
      s->delayl[s->dlk] = inl + outputl[i] * fb;

      //Right channel

      //compute the delay in samples using linear interpolation between the lfo delays
      s->mdel = (s->dr1 * (count - i) + s->dr2 * i) / count;
      if (++s->drk >= s->maxdelay)
	s->drk = 0;
      tmp = s->drk - s->mdel + s->maxdelay * 2.0;	//where should I get the sample from

      F2I (tmp, s->dlhi);
      s->dlhi %= s->maxdelay;

      s->dlhi2 = (s->dlhi - 1 + s->maxdelay) % s->maxdelay;
      s->dllo = 1.0 - fmodf (tmp, 1.0);
      outputr[i] = s->delayr[s->dlhi2] * s->dllo + s->delayr[s->dlhi] * (1.0 - s->dllo);
      s->delayr[s->dlk] = inr + outputr[i] * fb;

    };

    if (subtract != 0)
    for (i = 0; i < count; i++)
      {
	outputl[i] *= -1.0;
	outputr[i] *= -1.0;
      };


  for (i = 0; i < count; i++)
    {
      outputl[i] *= panning*vol;
      outputr[i] *= (1.0 - panning)*vol;
    };
};

/*
 * Cleanup the effect
 */

void
Chorus_cleanup(ZChorus_t * s)
{
  int i;
  
  for (i = 0; i < s->maxdelay; i++)
    {
      s->delayl[i] = 0.0;
      s->delayr[i] = 0.0;
    };

};



