/* tuneit.c -- Detect fundamental frequency of a sound
* Copyright (C) 2004, 2005  Mario Lang <mlang@delysid.org>
*
* Modified for holharm by Josep Andreu
* recognizer.h  Recognizer Audio Note definitions
*
* This is free software, placed under the terms of the
* GNU General Public License, as published by the Free Software Foundation.
* Please see the file COPYING for details.
*/
/*
  holharm - a inteligent harmonizer DSSI plugin software                                          
  
    recognizenote.c  -  Harmonizer functions.
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


#include "harmonizer.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>


void
Recognize_init (holharm_t * s, float *efxoutl_, float *efxoutr_)
{
  int i;

  s->rn.recinl = efxoutl_;
  s->rn.recinl = efxoutr_;


  float aFreq = 440.0;
  static const char *englishNotes[12] =
    { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };
  s->rn.notes = englishNotes;
  s->rn.ultima = -1;
  s->rn.note = 0;
  s->rn.nfreq = 0;
  s->rn.afreq = 0;
  s->rn.freqs[0] = aFreq;
  s->rn.lfreqs[0] = logf (s->rn.freqs[0]);
  for (i = 1; i < 12; i++)
    {
      s->rn.freqs[i] = s->rn.freqs[i - 1] * D_NOTE;
      s->rn.lfreqs[i] = s->rn.lfreqs[i - 1] + LOG_D_NOTE;
    }



  s->rn.schmittPointer = s->rn.schmittBuffer;
  memset (s->rn.outi, 0, sizeof (float) * 2048);

}


void
schmittS16LE (holharm_t * s, signed short int *indata, unsigned long count)
{
  int i, j;
  float trigfact = 0.6;


  for (i = 0; i < count; i++)
    {
      *s->rn.schmittPointer++ = indata[i];
      if (s->rn.schmittPointer - s->rn.schmittBuffer >= s->rn.blockSize)
	{
	  int endpoint, startpoint, t1, t2, A1, A2, tc, schmittTriggered;

	  s->rn.schmittPointer = s->rn.schmittBuffer;

	  for (j = 0, A1 = 0, A2 = 0; j < s->rn.blockSize; j++)
	    {
	      if (s->rn.schmittBuffer[j] > 0 && A1 < s->rn.schmittBuffer[j])
		A1 = s->rn.schmittBuffer[j];
	      if (s->rn.schmittBuffer[j] < 0 && A2 < -s->rn.schmittBuffer[j])
		A2 = -s->rn.schmittBuffer[j];
	    }
	  t1 = (int) (A1 * trigfact + 0.5);
	  t2 = -(int) (A2 * trigfact + 0.5);
	  startpoint = 0;
	  for (j = 1; s->rn.schmittBuffer[j] <= t1 && j < s->rn.blockSize;
	       j++);
	  for (;
	       !(s->rn.schmittBuffer[j] >= t2
		 && s->rn.schmittBuffer[j + 1] < t2) && j < s->rn.blockSize;
	       j++);
	  startpoint = j;
	  schmittTriggered = 0;
	  endpoint = startpoint + 1;
	  for (j = startpoint, tc = 0; j < s->rn.blockSize; j++)
	    {
	      if (!schmittTriggered)
		{
		  schmittTriggered = (s->rn.schmittBuffer[j] >= t1);
		}
	      else if (s->rn.schmittBuffer[j] >= t2
		       && s->rn.schmittBuffer[j + 1] < t2)
		{
		  endpoint = j;
		  tc++;
		  schmittTriggered = 0;
		}
	    }
	  if (endpoint > startpoint)
	    {
	      s->rn.afreq =
		(float) s->SAMPLE_RATE * (tc /
					  (float) (endpoint - startpoint));
	      displayFrequency (s, s->rn.afreq);

	    }
	}
    }
};


void
schmittFloat (holharm_t * s, float *indatal, float *indatar,
	      unsigned long count)
{
  int i;

  for (i = 0; i < count; i++)
    {
      s->rn.outi[i] = (short) ((indatal[i] + indatar[i]) * 32768);
    }

  s->rn.blockSize = s->SAMPLE_RATE / count;
  schmittS16LE (s, s->rn.outi, count);
};



void
displayFrequency (holharm_t * s, float freq)
{
  float ldf, mldf;
  float lfreq;
  int i;
  int noteoff = 0;
  int octave = 4;

  if (freq < 1E-15)
    freq = 1E-15;
  lfreq = logf (freq);
  while (lfreq < s->rn.lfreqs[0] - LOG_D_NOTE * .5)
    lfreq += LOG_2;
  while (lfreq >= s->rn.lfreqs[0] + LOG_2 - LOG_D_NOTE * .5)
    lfreq -= LOG_2;
  mldf = LOG_D_NOTE;
  for (i = 0; i < 12; i++)
    {
      ldf = fabsf (lfreq - s->rn.lfreqs[i]);
      if (ldf < mldf)
	{
	  mldf = ldf;
	  s->rn.note = i;
	}
    }

  s->rn.nfreq = s->rn.freqs[s->rn.note];
  while (s->rn.nfreq / freq > D_NOTE_SQRT)
    {
      s->rn.nfreq *= .5;
      octave--;
      if (octave < -1)
	{
	  noteoff = 1;
	  break;
	}

    }
  while (freq / s->rn.nfreq > D_NOTE_SQRT)
    {
      s->rn.nfreq *= 2.0;
      octave++;
      if (octave > 7)
	{
	  noteoff = 1;
	  break;
	}
    }


  if (!noteoff)
    s->rc.reconota = 24 + (octave * 12) + s->rn.note - 3;

};
