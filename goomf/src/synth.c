/*
  goomf - a FM mono synth plugin software

  synth.c  -  synthesizer functions
  Copyright (C) 2009 Josep Andreu (Holborn)
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


#include <math.h>
#include "goomf.h"


void
init_vars (goomf_synth_t * s)
{

  int i;

  //Init de vars
  s->gate = 0;
  s->env_time = 0.0f;
  s->renv_time = 0.0f;
  s->lfol = 0.0f;
// FM Operator frequencys

  s->lasfreq[0] = 2.0;
  s->lasfreq[1] = 1.2;
  s->lasfreq[2] = 1.0;
  s->lasfreq[3] = 1.5;
  s->lasfreq[4] = 2.0;
  s->lasfreq[5] = 2.5;
  s->lasfreq[6] = 3.0;
  s->lasfreq[7] = 4.0;
  s->lasfreq[8] = 4.5;
  s->lasfreq[9] = 5.0;
  s->lasfreq[10] = 6.0;
  s->lasfreq[11] = 7.0;
  s->lasfreq[12] = 7.5;
  s->lasfreq[13] = 8.0;
  s->lasfreq[14] = 9.0;
  s->lasfreq[15] = 10.0;
  s->lasfreq[16] = 10.5;
  s->lasfreq[17] = 11.0;
  s->lasfreq[18] = 12.0;
  s->lasfreq[19] = 13.0;
  s->lasfreq[20] = 14.0;
  s->lasfreq[21] = 16.0;

  s->tune = 0;
  s->pitch = 0.0f;


// Init frequency Notes 


  for (i = 1; i <= 192; i++)
    {
      s->h[i].f1 = 8.1757989156 * expf ((float) (i - 2) * logf (2.0) / 12.0);
      s->h[i].f2 = 8.1757989156 * expf ((float) (i) * logf (2.0) / 12.0);
      s->h[i].f3 = 8.1757989156 * expf ((float) (i + 2) * logf (2.0) / 12.0);
    }


// Allocate memory for calculated sins

  size_t sizesin = (size_t) (D_PI * 1000) + 2;


  memset (s->lsin, 0, sizeof (float) * (sizesin + 4));
  memset (s->nsin, 0, sizeof (float) * (sizesin + 4));
  memset (s->msin, 0, sizeof (float) * (sizesin + 4));
  memset (s->psin, 0, sizeof (float) * (sizesin + 4));
  memset (s->qsin, 0, sizeof (float) * (sizesin + 4));
  memset (s->rsin, 0, sizeof (float) * (sizesin + 4));
  memset (s->tsin, 0, sizeof (float) * (sizesin + 4));
  memset (s->ssin, 0, sizeof (float) * (sizesin + 4));
  memset (s->usin, 0, sizeof (float) * (sizesin + 4));



  float x_sin;
  for (i = 0; i < (int) sizesin; i++)

    {

      x_sin = (float) (i * D_PI / sizesin);
      s->lsin[i] = sinf (x_sin);

      if (i > 0)
	s->lsin[i - 1] =
	  (s->lsin[i - 1] * (1.0 + s->lsin[i] - s->lsin[i - 1]));
      if (i > 1)
	s->lsin[i - 2] =
	  (s->lsin[i - 2] * (1.0 + s->lsin[i - 1] - s->lsin[i - 2]));
      if (i > 2)
	s->lsin[i - 3] =
	  (s->lsin[i - 3] * (1.0 + s->lsin[i - 2] - s->lsin[i - 3]));
      if (i > 3)
	s->lsin[i - 4] =
	  (s->lsin[i - 4] * (1.0 + s->lsin[i - 3] - s->lsin[i - 4]));
      if (i > 4)
	s->lsin[i - 5] =
	  (s->lsin[i - 5] * (1.0 + s->lsin[i - 4] - s->lsin[i - 5]));
      if (i > 5)
	s->lsin[i - 6] =
	  (s->lsin[i - 6] * (1.0 + s->lsin[i - 5] - s->lsin[i - 6]));
      if (i > 6)
	s->lsin[i - 7] =
	  (s->lsin[i - 7] * (1.0 + s->lsin[i - 6] - s->lsin[i - 7]));
      if (i > 7)
	s->lsin[i - 8] =
	  (s->lsin[i - 8] * (1.0 + s->lsin[i - 7] - s->lsin[i - 8]));


    }

  for (i = 0; i < (int) sizesin; i++)
    {

      x_sin = (float) (i * D_PI / sizesin);

      s->nsin[i] = sinf (x_sin + sinf (x_sin));
      s->msin[i] = sinf (x_sin + sinf (1.5 * x_sin));
      if (i > 0)
	s->msin[i - 1] =
	  (s->msin[i - 1] * (1.0 + s->msin[i] - s->msin[i - 1]));
      if (i > 1)
	s->msin[i - 2] =
	  (s->msin[i - 2] * (1.0 + s->msin[i - 1] - s->msin[i - 2]));
      if (i > 2)
	s->msin[i - 3] =
	  (s->msin[i - 3] * (1.0 + s->msin[i - 2] - s->msin[i - 3]));
      if (i > 3)
	s->msin[i - 4] =
	  (s->msin[i - 4] * (1.0 + s->msin[i - 3] - s->msin[i - 4]));
      if (i > 4)
	s->msin[i - 5] =
	  (s->msin[i - 5] * (1.0 + s->msin[i - 4] - s->msin[i - 5]));
      if (i > 5)
	s->msin[i - 6] =
	  (s->msin[i - 6] * (1.0 + s->msin[i - 5] - s->msin[i - 6]));
      if (i > 6)
	s->msin[i - 7] =
	  (s->msin[i - 7] * (1.0 + s->msin[i - 6] - s->msin[i - 7]));
      if (i > 7)
	s->msin[i - 8] =
	  (s->msin[i - 8] * (1.0 + s->msin[i - 7] - s->msin[i - 8]));

      s->psin[i] = sinf (x_sin + sinf (2.0 * x_sin));
      s->qsin[i] = sinf (s->nsin[i] + s->lsin[i] + s->psin[i]);
      s->rsin[i] = sinf (s->msin[i] + sinf (s->msin[i]));
      s->tsin[i] = sinf (s->msin[i] + sinf (s->nsin[i]));

      s->ssin[i] = sinf( x_sin + sinf(3.0 * x_sin));
      if (i > 0)
	s->ssin[i - 1] =
	  (s->ssin[i - 1] * (1.0 + s->ssin[i] - s->ssin[i - 1]));
      if (i > 1)
	s->ssin[i - 2] =
	  (s->ssin[i - 2] * (1.0 + s->ssin[i - 1] - s->ssin[i - 2]));
      if (i > 2)
	s->ssin[i - 3] =
	  (s->ssin[i - 3] * (1.0 + s->ssin[i - 2] - s->ssin[i - 3]));
      if (i > 3)
	s->ssin[i - 4] =
	  (s->ssin[i - 4] * (1.0 + s->ssin[i - 3] - s->ssin[i - 4]));
      if (i > 4)
	s->ssin[i - 5] =
	  (s->ssin[i - 5] * (1.0 + s->ssin[i - 4] - s->ssin[i - 5]));
      if (i > 5)
	s->ssin[i - 6] =
	  (s->ssin[i - 6] * (1.0 + s->ssin[i - 5] - s->ssin[i - 6]));
      if (i > 6)
	s->ssin[i - 7] =
	  (s->ssin[i - 7] * (1.0 + s->ssin[i - 6] - s->ssin[i - 7]));
      if (i > 7)
	s->ssin[i - 8] =
	  (s->ssin[i - 8] * (1.0 + s->ssin[i - 7] - s->ssin[i - 8]));

      s->usin[i] = sinf (x_sin + sinf(2.0 * s->ssin[i]));

    }


  for (i = 0; i < (int) sizesin; i++)
    {
      x_sin = (float) (i * D_PI / sizesin);
      s->ssin[i] = sinf (x_sin + s->usin[i]);

    }


// Init Buffers


  memset (s->bufl, 0, sizeof (float) * 8192);
  memset (s->bufr, 0, sizeof (float) * 8192);

  for(i=0; i<6; i++) s->active[i] = 0;


  AnalogFilter_Init (s, &s->Fl, 0, 21.0, 1, 0);
  AnalogFilter_Init (s, &s->Fr, 0, 21.0, 1, 0);


};




void
Adjust_Audio(goomf_synth_t * s)
{
  s->increment = 1.0 / s->SAMPLE_RATE;
  s->D_PI_to_SAMPLE_RATE = D_PI / s->SAMPLE_RATE;
};

// Returns the FM Operator Pitch (Frequency + LFO)

float
pitch_Operator (goomf_synth_t * s, int i)
{
  int H = (int) *(s->H[i]);
  float HF = (float) *(s->HF[i]);
  return (s->lasfreq[H] + HF);
}


float
pitch_Operator2 (goomf_synth_t * s, int i)
{
  int H = (int) *(s->H[i]);
  float HF = (float) *(s->HF[i]);
  return (s->lasfreq[H] - HF);
}



float
Jenvelope (goomf_synth_t * s, int op)
{

  LADSPA_Data attack = *(s->attack[op]);
  LADSPA_Data decay = *(s->decay[op]);
  LADSPA_Data sustain = *(s->sustain[op]);
  LADSPA_Data release = *(s->release[op]);
  LADSPA_Data volume = *(s->Ovol[op]);
  LADSPA_Data coef = volume * s->velocity;
  float t = s->env_time;
  float r = s->renv_time;
  float tmp = 0.0f;



  if (s->gate)
    {
      if (t > attack + decay)
	return (sustain);

      if (t > attack)
	return (1.0 - (1.0 - sustain) * (t - attack) / decay);

      if (s->Env_Vol[op] > (t / attack * coef))
	return (s->Env_Vol[op] / coef );
      else
	return (t / attack);

    }
  else
    {
      if (r <release)
        {
	tmp =s->Env_Vol[op] * (1.0 - r / release);
	if (tmp<0.000000001)
	  { 
	  clear_synth (s, op);
	  return (0.0f);
          }
        else return (tmp);
        }
         else
	{
	  clear_synth (s, op);
	  return (0.0f);
	}
    }

};


float
Fenvelope (goomf_synth_t * s, int op)
{

  LADSPA_Data attack = *(s->attack[op]);
  LADSPA_Data decay = *(s->decay[op]);
  LADSPA_Data sustain = *(s->sustain[op]);
  LADSPA_Data release = *(s->release[op]);
  float t = s->env_time;
  float r = s->renv_time;

  if (s->gate)
    {
      if (t > attack + decay)
	return (sustain);
      if (t > attack)
	return (1.0 - (1.0 - sustain) * (t - attack) / decay);
	return (t / attack);
    }
  else
    {
      if (r < release)
	return (s->FEnv_Vol * (1.0 - r / release));
      else
	return (0.0f);
    }

}

void
clear_synth (goomf_synth_t * s, int op)
{
  int i;
  int sum = 0;
  

  if(s->filt_cleared) return;
  if(!s->active[op]) return;

  s->active[op]= 0;

  for(i=0;i<6;i++) if (*(s->Ovol[i])>0.0f) sum += s->active[i]; 
 

  if ((!sum) && (!s->filt_cleared))
  {
  AnalogFilter_Cleanup (s, &s->Fl);
  AnalogFilter_Cleanup (s, &s->Fr);
  s->FEnv_Vol = 0.0f;
  s->velocity = 0.0f;
  s->filt_cleared=1;
  }
}



// Returns Pitch LFO 


float
Pitch_LFO (goomf_synth_t * s, float t, int type)
{

  float x, out, lfoinc;
  LADSPA_Data LFO_Delay = *(s->LFO_Delay);
  LADSPA_Data LFO_Frequency = *(s->LFO_Frequency);
  int LFO_Wave = *(s->LFO_Wave);


  if (t < LFO_Delay)
    return (0.0f);

  if (type)
    {
      lfoinc = fabsf (LFO_Frequency) * s->D_PI_to_SAMPLE_RATE;
      if (lfoinc > 0.49999999)
	lfoinc = 0.499999999;
      x = s->lfol * D_PI;
      s->lfol += lfoinc;
      if (s->lfol > 1.0)
	s->lfol -= 1.0;
    }
  else
    {
      x = fmodf (LFO_Frequency * 8.0 * t, 1.0) * D_PI;
    }
  out = NFsin (s, LFO_Wave, x);

  return (out);

}


// Return Played Note Frequency

float
Get_Partial (goomf_synth_t * s)
{
  int l;
  float partial = 0;
  float freq_note = 0;
  float portdir;
  LADSPA_Data Portamento = *(s->portamento);

  if ((s->note != s->lastnote) || (s->pitch != 0))
    {
      s->lpartial = s->apartial;
      s->lastnote = s->note;
      l = s->note;
      freq_note =
	(s->pitch >
	 0) ? s->h[l].f2 + (s->h[l].f3 - s->h[l].f2) * s->pitch : s->h[l].f2 +
	(s->h[l].f2 - s->h[l].f1) * s->pitch;
      partial =
	(1.0 + *(s->tune) / 16.0) * freq_note * s->D_PI_to_SAMPLE_RATE;
      if (partial > D_PI)
	partial = fmodf (partial, D_PI);
      s->apartial = partial;
    }

  if (s->lpartial < s->apartial)
    portdir = 1.0;
  else
    portdir = -1.0;

  s->ppartial += portdir * s->increment * Portamento * 1000.0;

  if ((portdir == 1.0) && (s->ppartial >= s->apartial))
    s->ppartial = s->apartial;
  if ((portdir == -1.0) && (s->ppartial <= s->apartial))
    s->ppartial = s->apartial;

  if (Portamento > 0)
    return (s->ppartial);
  else
    return (s->apartial);

};


float
NFsin (goomf_synth_t * s, int i, float x)
{

  if (x > D_PI)
    x = fmodf (x, D_PI);

  long int k = F2I (x * 1000.0);



  if (i == 1)
    return (s->lsin[k]);
  if (i == 2)
    return (s->nsin[k]);
  if (i == 3)
    return (s->msin[k]);
  if (i == 4)
    return (s->psin[k]);
  if (i == 5)
    return (s->qsin[k]);
  if (i == 6)
    return (s->rsin[k]);
  if (i == 7)
    return (s->tsin[k]);
  if (i == 8)
    return (s->ssin[k]);
  if (i == 9)
    return (s->usin[k]);



  return 0.0;
}


// Main Audio thread

void
Alg1s (goomf_synth_t * s, int nframes)
{

  int i;
  int l1;
  float sound, sound2, filt, filt2;
  float m_partial=0.0f;
  LADSPA_Data volumen;
  int Ftype = F2I(*(s->Ftype));
  LADSPA_Data Fgain = *(s->Fgain);
  LADSPA_Data Fcutoff = *(s->Fcutoff);
  LADSPA_Data Fq = *(s->Fq);
  int Fstages = F2I(*(s->Fstages));
  LADSPA_Data FLFO = *(s->FLFO);
  int FADSR = F2I(*(s->FADSR));
  int VELO = F2I(*(s->Fvelocity));
  int algo = F2I(*(s->algo));
  LADSPA_Data freq = 0.0f;
  LADSPA_Data tmp,oldfreq;
  int wave = 0;
  int wave1 = 0;
  LADSPA_Data pLFO;
  LADSPA_Data LFO=0.0f;


  LADSPA_Data LFO_Volume = *(s->LFO_Volume) * .5;

  if (!s->filt_cleared) m_partial = Get_Partial (s);

  for (l1 = 0; l1 < nframes; l1++)
    {

      sound = 0.0f;
      sound2 = 0.0f;
      filt = 0.0f;
      filt2 = 0.0f;

      if (!s->filt_cleared) LFO = Pitch_LFO (s, s->env_time, 0) * LFO_Volume * s->modulation;

      for (i = 0; i < 6; i += 2)
	{
	  volumen = *(s->Ovol[i]);

	  if ((s->active[i]) && (volumen > 0.0f))
	    {
	      pLFO = (float) *(s->pLFO[i]) * LFO;
	      wave = F2I(*(s->wave[i]));
              if ((algo==2) && (wave==1)) wave = 2;

	      if (s->gate)
		{
		  s->Envelope_Volume[i] = Jenvelope (s, i);
		  s->Env_Vol[i] =
		    s->velocity * volumen * s->Envelope_Volume[i];
		}
	      else
		s->Env_Vol[i] = Jenvelope (s, i);

	      //L
	      s->f[i].dphi = m_partial * (pitch_Operator (s, i) + pLFO);
	      if (s->f[i].dphi > D_PI)
		s->f[i].dphi -= D_PI;
	      s->f[i].phi += s->f[i].dphi;
	      if (s->f[i].phi > D_PI)
		s->f[i].phi -= D_PI;
         
	      //R 
	      s->f[i].dphi2 = m_partial * (pitch_Operator2 (s, i) + pLFO);
	      if (s->f[i].dphi2 > D_PI)
		s->f[i].dphi2 -= D_PI;
	      s->f[i].phi2 += s->f[i].dphi2;
	      if (s->f[i].phi2 > D_PI)
		s->f[i].phi2 -= D_PI;
          
             }
        
          volumen = *(s->Ovol[i+1]);

	  if ((s->active[i+1]) && (volumen > 0.0f))
	    {
	      pLFO = (float) *(s->pLFO[i+1]) * LFO;
	      wave1 = F2I(*(s->wave[i+1]));

	      if (s->gate)
		{
		  s->Envelope_Volume[i+1] = Jenvelope (s, i+1);
		  s->Env_Vol[i+1] =
		    s->velocity * volumen * s->Envelope_Volume[i+1];
		}
	      else
		s->Env_Vol[i+1] = Jenvelope (s, i+1);

	      //L
	      if (algo == 1)
	      s->f[i+1].dphi = m_partial * (pitch_Operator (s, i+1) + pLFO);
	      else 
	      s->f[i+1].dphi = s->f[i].dphi * (pitch_Operator (s, i+1) + pLFO);
	      if (s->f[i+1].dphi > D_PI)
		s->f[i+1].dphi -= D_PI;
	      s->f[i+1].phi += s->f[i+1].dphi;
	      if (s->f[i+1].phi > D_PI)
		s->f[i+1].phi -= D_PI;
           
	      //R 
	      if (algo==1)
	      s->f[i+1].dphi2 = m_partial * (pitch_Operator2 (s, i+1) + pLFO);
	      else 
	      s->f[i+1].dphi2 = s->f[i].dphi2 * (pitch_Operator2 (s, i+1) + pLFO);
	      if (s->f[i+1].dphi2 > D_PI)
		s->f[i+1].dphi2 -= D_PI;
	      s->f[i+1].phi2 += s->f[i+1].dphi2;
	      if (s->f[i+1].phi2 > D_PI)
		s->f[i+1].phi2 -= D_PI;
            
             
             }             
             
              if (algo==2)
              {              
	      sound += s->Env_Vol[i] * NFsin (s, wave, s->f[i].phi + s->Env_Vol[i+1]*NFsin(s,wave1,s->f[i+1].phi));
	      sound2 += s->Env_Vol[i] * NFsin (s, wave, s->f[i].phi2 + s->Env_Vol[i+1]*NFsin(s,wave1,s->f[i+1].phi2));
	      }
	      else
	      {
              sound += s->Env_Vol[i] * NFsin (s, wave, s->f[i].phi);
              sound += s->Env_Vol[i+1] * NFsin (s, wave1, s->f[i+1].phi);
              sound2 += s->Env_Vol[i] * NFsin (s, wave, s->f[i].phi2);
	      sound2 += s->Env_Vol[i+1] * NFsin (s, wave1, s->f[i+1].phi2);
              }
	
	}


      if (s->Rtype != Ftype)
	{
	  s->Rtype = Ftype;
	  settype (s, &s->Fl, (int) s->Rtype);
	  settype (s, &s->Fr, (int) s->Rtype);
	}

      if (s->Rgain != Fgain)
	{
	  s->Rgain = Fgain;
	  setgain (s, &s->Fl, s->Rgain*12.0);
	  setgain (s, &s->Fr, s->Rgain*12.0);
	}

      if (s->Rcutoff != Fcutoff)
	{
	  s->Rcutoff = Fcutoff;
	  setfreq (s, &s->Fl, s->Rcutoff);
	  setfreq (s, &s->Fr, s->Rcutoff);
	}

      if (s->Rq != Fq)
	{
	  s->Rq = Fq;
	  tmp = powf (30.0, s->Rq);
	  setq (s, &s->Fl, tmp);
	  setq (s, &s->Fr, tmp);
	}

      if (s->Rstages != Fstages)
	{
	  s->Rstages = Fstages;
	  setstages (s, &s->Fl, s->Rstages);
	  setstages (s, &s->Fr, s->Rstages);
	}

      freq = s->Rcutoff;

      if(!s->filt_cleared)

      { 
      if (VELO)
	freq *= s->velocity;
      if (FADSR)
	{
	  s->FEnv_Vol = Fenvelope (s, FADSR - 1);
	  oldfreq = freq;
	  freq *= s->FEnv_Vol;
	  if (freq < (s->Rcutoff *.33)) freq=oldfreq;
	  
	}
      if (FLFO > 0.0f)
	{
	  freq += Pitch_LFO (s, s->env_time, 0) * FLFO * freq;
	}

      if (freq < 20.0f)
	freq = 20.0;
      if (freq > 10020.0)
	freq = 10020.0;

      setfreq (s, &s->Fl, freq);
      setfreq (s, &s->Fr, freq);

      filt = sound;
      filt2 = sound2;
      
      filterout (s, &s->Fl, &filt, 1);
      filterout (s, &s->Fr, &filt2, 1);

      }
      
      s->bufl[l1] = (sound * (1.0 - Fgain)) + (filt * Fgain);
      s->bufr[l1] = (sound2 * (1.0 - Fgain)) + (filt2 * Fgain);

      s->env_time += s->increment;
      s->renv_time += (s->increment * .0001);


    }


}

