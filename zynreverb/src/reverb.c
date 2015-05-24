/*
  ZynAddSubFX - a software synthesizer
 
  Reverb.C - Reverberation effect
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

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "reverb.h"

/*TODO: EarlyReflections,Prdelay,Perbalance */
void
Reverb_Init(ZReverb_t * s)
{
  s->roomsize = 1.0;
  s->rs = 1.0;
  s->idelaylen = 40000;

  Reverb_settype(s);

  Reverb_Cleanup(s);			//do not call this before the comb initialisation

  AnalogFilter_Init(s,&s->lpf,2,10020.0,1,0);
 
  AnalogFilter_Init(s,&s->hpf,3,21.0,1,0);
   
};


/*
 * Cleanup the effect
 */
void
Reverb_Cleanup (ZReverb_t * s)
{
  int i, j;

  for (i = 0; i < REV_COMBS * 2; i++)
    {
      s->lpcomb[i] = 0.0;
      for (j = 0; j < s->comblen[i]; j++)
      s->comb[i][j] = 0.0;
    };


  for (i = 0; i < REV_APS * 2; i++)
     {
       for (j = 0; j < s->aplen[i]; j++) s->ap[i][j]=0.0;
     }

    for (i = 0; i < s->idelaylen; i++) s->idelay[i] = 0.0;

    AnalogFilter_Cleanup (s,&s->hpf);
    AnalogFilter_Cleanup (s,&s->lpf); 


};

/*
 * Process one channel; 0=left,1=right
 */
void
processmono (ZReverb_t * s, int ch, float * output, unsigned long count)
{
  int i, j;
  float fbout, tmp;
  //TODO: implement the high part from lohidamp

  for (j = REV_COMBS * ch; j < REV_COMBS * (ch + 1); j++)
    {

      int ck = s->combk[j];
      int comblength = s->comblen[j];
      float lpcombj = s->lpcomb[j];

      for (i = 0; i < count; i++)
	{
	  fbout = s->comb[j][ck] * s->combfb[j];
	  fbout = fbout * (1.0 - s->lohifb) + (lpcombj * s->lohifb);
	  lpcombj = fbout;

	  s->comb[j][ck] = s->inputbuf[i] + fbout;
	   output[i] += fbout;

	  if ((++ck) >= comblength)
	    ck = 0;
	};

      s->combk[j] = ck;
      s->lpcomb[j] = lpcombj;
    };


  for (j = REV_APS * ch; j < REV_APS * (1 + ch); j++)
    {
      int ak = s->apk[j];
      int aplength = s->aplen[j];
      for (i = 0; i < count; i++)
	{
	  tmp = s->ap[j][ak];
	  s->ap[j][ak] = 0.7 * tmp + output[i];
	  output[i] = tmp - 0.7 * s->ap[j][ak];
	  if ((++ak) >= aplength)
	    ak = 0;
	};
      s->apk[j] = ak;
    };



};

/*
 * Effect output
 */
void
out (ZReverb_t * s, float * smps_l, float * smps_r, unsigned long count)
{
  int i;

  LADSPA_Data *const outputl = s->efxoutl;
  LADSPA_Data *const outputr = s->efxoutr;
  LADSPA_Data pan = *(s->panning);
  LADSPA_Data idelayfb = *(s->idelayfb);
  LADSPA_Data delay = *(s->Pidelay);
  LADSPA_Data Ptime = *(s->Ptime);
  LADSPA_Data Plohidamp = *(s->Plohidamp);
  LADSPA_Data Proomsize = *(s->Proomsize);
  LADSPA_Data Plpf = *(s->Plpf);
  LADSPA_Data Phpf = *(s->Phpf);
  LADSPA_Data vol = *(s->outvolume);  
  LADSPA_Data dry = *(s->dry);
  
  if (s->Rlohidamp != Plohidamp)
     {
        s->Rlohidamp = Plohidamp;
        Reverb_setlohidamp(s, s->Rlohidamp);
     }   


  if ( s->Rtime != Ptime)
    {
       s->Rtime = Ptime;
       Reverb_settime(s,s->Rtime);
    }   

  
  if (s->Rroomsize != Proomsize)
    {
       s->Rroomsize = Proomsize;
       Reverb_setroomsize(s,s->Rroomsize);
    }
         

  if (s->Rlpf != Plpf)
     {
       s->Rlpf = Plpf;
       setfreq(s,&s->lpf,s->Rlpf);
     }
     
  if (s->Rhpf != Phpf)     
     {
       s->Rhpf = Phpf;
       setfreq(s,&s->hpf,s->Rhpf);
     }
       

  s->idelaylen = (int) (s->SAMPLE_RATE * delay / 1000);


  for (i = 0; i < count; i++)
    {
      s->inputbuf[i] = (smps_l[i] + smps_r[i]) / 2.0;
      //Initial delay r
	  float tmp = s->inputbuf[i] + s->idelay[s->idelayk] * idelayfb;
	  s->inputbuf[i] = s->idelay[s->idelayk];
	  s->idelay[s->idelayk] = tmp;
	  s->idelayk++;
	  if (s->idelayk >= s->idelaylen)
	    s->idelayk = 0;
    }


   filterout (s,&s->lpf, s->inputbuf,count);
   filterout (s,&s->hpf, s->inputbuf,count);

  processmono (s, 0, outputl, count);	//left
  processmono (s, 1, outputr, count );	//right



  float lvol = s->rs / REV_COMBS * pan;
  float rvol = s->rs / REV_COMBS * (1.0 - pan);
  lvol *= 2;
  rvol *= 2;

  for (i = 0; i < count; i++)
    {
      outputl[i] *= lvol;
      outputr[i] *= rvol;
      outputl[i] *= vol;
      outputr[i] *= vol;
      outputl[i] += (s->indatal[i]*dry);
      outputr[i] += (s->indatar[i]*dry);

    }

   


};

void
Reverb_settime(ZReverb_t * s, float Ptime)
{
  int i;
  
  for (i = 0; i < REV_COMBS * 2; i++)
    {
      s->combfb[i] =
	-expf ((float) s->comblen[i] / (float) s->SAMPLE_RATE * logf (0.001) /
	       Ptime);
      //the feedback is negative because it removes the DC
    }
}

void
Reverb_setlohidamp (ZReverb_t * s, float Rlohidamp)
{

s->lohifb = Rlohidamp * Rlohidamp;

}


void
Reverb_sethpf (ZReverb_t * s, float RPhpf)
{
//	hpf = AnalogFilter (3, fr, 1, 0);
 	setfreq (s,&s->hpf,RPhpf);
   
}

void
Reverb_setlpf (ZReverb_t * s, float RPlpf)
{
	// lpf = AnalogFilter (2, fr, 1, 0);
      	setfreq (s,&s->lpf,RPlpf);
   
};

void
Reverb_settype (ZReverb_t * s)
{

  
  int i;
  int combtunings[REV_COMBS] = 
    //Freeverb by Jezar at Dreampoint
    {1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617};
  int aptunings[REV_APS] = 
    //Freeverb by Jezar at Dreampoint
    {225, 341, 441, 556};
  
  float tmp;

  for (i = 0; i < REV_COMBS * 2; i++)
    {
      tmp = combtunings[i % REV_COMBS];
      tmp *= s->roomsize;
      if (i > REV_COMBS)
	tmp += 23.0;
      tmp *= s->SAMPLE_RATE / 44100.0;	//adjust the combs according to the samplerate
      if (tmp < 10)
	tmp = 10;

      s->comblen[i] = (int) tmp;
      s->combk[i] = 0;
      s->lpcomb[i] = 0;
    };

  for (i = 0; i < REV_APS * 2; i++)
    {
      tmp = aptunings[i % REV_APS];
      tmp *= s->roomsize;
      if (i > REV_APS)
	tmp += 23.0;
      tmp *= s->SAMPLE_RATE / 44100.0;	//adjust the combs according to the samplerate
      if (tmp < 10)
	tmp = 10;
      s->aplen[i] = (int) tmp;
      s->apk[i] = 0;
    };

  // Reverb_settime (s,2.5);

};

void
Reverb_setroomsize(ZReverb_t * s, float Groomsize)
{

  if (Groomsize > 0.0)
    Groomsize *= 2.0;
  s->roomsize = powf (10.0, Groomsize);
  s->rs = sqrtf (s->roomsize);
  Reverb_settype (s);
};


