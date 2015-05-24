/*
  holharm - a inteligent harmonizer DSSI plugin software

  harmonizer.c  -  Harmonizer functions.
  Copyright (C) 2008-2009 Josep Andreu
  Author: Josep Andreu

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License
 (version2)  along with this program; if not, write to the Free Software
 Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include <string.h>
#include <stdlib.h>
#include "harmonizer.h"
#include "math.h"

void
Harmonizer_start (holharm_t * s)
{

  memset (s->outil, 0, sizeof (float) * 2048);
  memset (s->outol, 0, sizeof (float) * 2048);

  memset (s->outir, 0, sizeof (float) * 2048);
  memset (s->outor, 0, sizeof (float) * 2048);

  s->val_sum = 0;
  s->last = 0;
  s->lcount = 0;

  Harmonizer_cleanup (s);


}


void
Harmonizer_cleanup (holharm_t * s)
{
  int i;
  s->val_sum = 0;
  for (i = 0; i < 2048; i++)
    {
      s->outil[i] = 0.0;
      s->outol[i] = 0.0;
      s->outir[i] = 0.0;
      s->outor[i] = 0.0;
    }

}



void
out_smb (holharm_t * s, float *smpsl, float *smpsr,
	 unsigned long sample_count)
{

  int i;
  float elratiol, elratior;
  float i_sum = 1e-12;
  float temp_sum, val_i_sum;
  float tmp;


  for (i = 0; i < sample_count; i++)
    {
      s->outil[i] = smpsl[i];
      s->outir[i] = smpsr[i];
      tmp = fabsf (smpsr[i] + smpsl[i]);
      if (tmp > i_sum)
	i_sum = tmp;

    }

  temp_sum = CLAMP (rap2dB (i_sum), -48.0, 15.0);
  val_i_sum = .6 * s->val_sum + .4 * temp_sum;
  s->val_sum = val_i_sum;


  if (*(s->PSELECT) == 1)
    {
      elratiol = s->ratiol;
      elratior = s->ratior;
    }
  else
    {
      elratiol = powf (2, *(s->intervall) / 12.0);
      elratior = powf (2, *(s->intervalr) / 12.0);
    }


  smbPitchShiftL (s, elratiol, sample_count, 2048, s->hq,
		  (float) s->SAMPLE_RATE, s->outil, s->outol);
  smbPitchShiftR (s, elratior, sample_count, 2048, s->hq,
		  (float) s->SAMPLE_RATE, s->outir, s->outor);


}




void
setpreset (holharm_t * s, unsigned char npreset)
{

// A ver que hacemos aqui 

};
