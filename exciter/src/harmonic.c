/*
  exciter - a MIDI helped exciter DSSI plugin software

  harmonic.c  -  Main dssi file of the plugin
  This file is based in the harmonic_gen_1220.c by Steve Harris
  Copyright (C) 2008-2009 Josep Andreu (Holborn)
  Author: Josep Andreu

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2) for more details.

  You should have received a copy of the GNU General Public License
(version2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ladspa.h>
#include "exciter.h"


void
Harmonic_Init (exciter_t * s)
{
  int i;

  for (i = 0; i < 10; i++)
    s->Rmag[i] = 0.0f;

  s->rmag_1 = 0.0f;
  s->rmag_2 = 0.0f;
  s->rmag_3 = 0.0f;
  s->rmag_4 = 0.0f;
  s->rmag_5 = 0.0f;
  s->rmag_6 = 0.0f;
  s->rmag_7 = 0.0f;
  s->rmag_8 = 0.0f;
  s->rmag_9 = 0.0f;
  s->rmag_10 = 0.0f;

static int phfl[10]={12,19,24,28,31,34,36,38,40,42};
for (i=0; i<10; i++) s->harmonic_flist[i]=phfl[i];
static int phnl[5]={12,24,31,36,40};
for(i=0;i<5;i++) s->harmonic_nlist[i]=phnl[i];



  s->itm1l = 0.0f;
  s->itm1r = 0.0f;
  s->otm1l = 0.0f;
  s->otm1r = 0.0f;

  AnalogFilter_Init (s, &s->lhpf, 3, 21.0, 1, 0);
  AnalogFilter_Init (s, &s->rhpf, 3, 21.0, 1, 0);

  s->rfreq = 21.0;
  

}

/* Calculate Chebychev coefficents from partial magnitudes, adapted from
 * example in Num. Rec. */
void
chebpc (float c[], float d[])
{
  int k, j;
  float sv, dd[HARMONICS];

  for (j = 0; j < HARMONICS; j++)
    {
      d[j] = dd[j] = 0.0;
    }

  d[0] = c[HARMONICS - 1];

  for (j = HARMONICS - 2; j >= 1; j--)
    {
      for (k = HARMONICS - j; k >= 1; k--)
	{
	  sv = d[k];
	  d[k] = 2.0 * d[k - 1] - dd[k];
	  dd[k] = sv;
	}
      sv = d[0];
      d[0] = -dd[0] + c[j];
      dd[0] = sv;
    }

  for (j = HARMONICS - 1; j >= 1; j--)
    {
      d[j] = d[j - 1] - dd[j];
    }
  d[0] = -dd[0] + 0.5 * c[0];
}



void
calcula_mag (exciter_t * s)
{

  LADSPA_Data mag_1 = *(s->mag_1);
  LADSPA_Data mag_2 = *(s->mag_2);
  LADSPA_Data mag_3 = *(s->mag_3);
  LADSPA_Data mag_4 = *(s->mag_4);
  LADSPA_Data mag_5 = *(s->mag_5);
  LADSPA_Data mag_6 = *(s->mag_6);
  LADSPA_Data mag_7 = *(s->mag_7);
  LADSPA_Data mag_8 = *(s->mag_8);
  LADSPA_Data mag_9 = *(s->mag_9);
  LADSPA_Data mag_10 = *(s->mag_10);

  int i, j, k;
  int nota;
  float mag_fix = 0.0f;

  for (i = 0; i < 10; i++)
    s->Rmag[i] = 0.0f;

  for (i = 0; i < 12; i++)
    {
      if (s->rc.Ch[s->rc.ctipo][i] != 0)
	{
	  for (j = 0; j < 5; j++)
	    {
	      nota = s->harmonic_nlist[j] + i;
	      for (k = 0; k < 10; k++)
		{
		  if (nota == s->harmonic_flist[k])
		    {
		      s->Rmag[k] +=  0.05;	// A ver que hacemos con los pares/impares
		    }
		}
	    }
	}
    }


  s->Rmag[0] += mag_1;
  s->Rmag[1] += mag_2;
  s->Rmag[2] += mag_3;
  s->Rmag[3] += mag_4;
  s->Rmag[4] += mag_5;
  s->Rmag[5] += mag_6;
  s->Rmag[6] += mag_7;
  s->Rmag[7] += mag_8;
  s->Rmag[8] += mag_9;
  s->Rmag[8] += mag_10;

  float mag[HARMONICS] =
    { 0.0f, s->Rmag[0], s->Rmag[1], s->Rmag[2], s->Rmag[3], s->Rmag[4], s->Rmag[5],
    s->Rmag[6], s->Rmag[7], s->Rmag[8], s->Rmag[9]
  };

  // Normalise magnitudes

  for (i = 0; i < 10; i++)
    mag_fix += fabs (s->Rmag[i]);


  if (mag_fix < 1.0f)
    {
      mag_fix = 1.0f;
    }
  else
    {
      mag_fix = 1.0f / mag_fix;
    }
  for (i = 0; i < HARMONICS; i++)
    {
      mag[i] *= mag_fix;
    }

  // Calculate polynomial coefficients, using Chebychev aproximation
  chebpc (mag, s->p);

}

void
out_exciter (exciter_t * s, float *smpsl, float *smpsr, unsigned long count)
{

  LADSPA_Data *const outputl = s->efxoutl;
  LADSPA_Data *const outputr = s->efxoutr;
  LADSPA_Data dry = *(s->dry);
  LADSPA_Data vol = *(s->outvolume);
  LADSPA_Data pan = *(s->panning);

  int i;

  for (i = 0; i < count; i++)
    {
      s->inputbufl[i] = smpsl[i];
      s->inputbufr[i] = smpsr[i];
    }

  filterout (s, &s->lhpf, s->inputbufl, count);
  filterout (s, &s->rhpf, s->inputbufr, count);

  for (i = 0; i < count; i++)
    {
      float xl = s->inputbufl[i];
      float xr = s->inputbufr[i];
      float yl, yr;

      // Calculate the polynomial using Horner's Rule
      yl = s->p[0] + (s->p[1] + (s->p[2] + (s->p[3] + (s->p[4] + (s->p[5] + (s->p[6] + (s->p[7] +
								   (s->p[8] +
								    (s->p[9] +
								     s->p[10] *
								     xl) *
								    xl) *
								   xl) * xl) *
						   xl) * xl) * xl) * xl) *
		   xl) * xl;

      yr = s->p[0] + (s->p[1] + (s->p[2] + (s->p[3] + (s->p[4] + (s->p[5] + (s->p[6] + (s->p[7] +
								   (s->p[8] +
								    (s->p[9] +
								     s->p[10] *
								     xr) *
								    xr) *
								   xr) * xr) *
						   xr) * xr) * xr) * xr) *
		   xr) * xr;

      // DC offset remove (odd harmonics cause DC offset)
      s->otm1l = 0.999f * s->otm1l + yl - s->itm1l;
      s->itm1l = yl;
      s->otm1r = 0.999f * s->otm1r + yr - s->itm1r;
      s->itm1r = yr;

      outputl[i] = (smpsl[i] * dry) + (s->otm1l * vol * pan);
      outputr[i] = (smpsr[i] * dry) + (s->otm1r * vol * (1.0 - pan));
     }

}



