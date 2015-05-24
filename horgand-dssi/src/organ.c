/*
  horgand - a organ software

 organ.C  -  organ functions
  Copyright (C) 2003-2004 Josep Andreu (Holborn)
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
#include "Holrgan.h"


void
init_vars (horgand_synth_t * s)
{

  //Init de vars
  s->rperhis = 65100;
  s->capsg = 0;
  s->cl_counter = 0;
  s->Master_Volume = 0.70f;

// FM Operator frequencys

  s->lasfreq[1] = 0.5f;
  s->lasfreq[2] = 0.75f;
  s->lasfreq[3] = 1.0f;
  s->lasfreq[4] = 1.5f;
  s->lasfreq[5] = 2.0f;
  s->lasfreq[6] = 2.5f;
  s->lasfreq[7] = 3.0f;
  s->lasfreq[8] = 4.0f;
  s->lasfreq[9] = 4.5f;
  s->lasfreq[10] = 5.0f;
  s->lasfreq[11] = 6.0f;
  s->lasfreq[12] = 7.0f;
  s->lasfreq[13] = 7.5f;
  s->lasfreq[14] = 8.0f;
  s->lasfreq[15] = 9.0f;
  s->lasfreq[16] = 10.0f;
  s->lasfreq[17] = 10.5f;
  s->lasfreq[18] = 11.0f;
  s->lasfreq[19] = 12.0f;
  s->lasfreq[20] = 13.0f;
  s->lasfreq[21] = 14.0f;
  s->lasfreq[22] = 16.0f;

  New (s);


  s->tune = 0;
  s->release = 0.6f;
  s->u_release = 1.0f / s->release;
  s->transpose = 0;
  s->pitch = 0.0f;
  s->pedal = 0;
  s->Stereo_Side = 0;
  s->To_Stereo_Side = 0;
  s->Rotary_X = 0.0f;
  s->Chorus_X_L = 0.0f;
  s->Chorus_X_R = 0.5f;
  s->Reverb_Time = 10;
  s->Reverb_Diffussion = 0.1f;
  s->Reverb_Volume = 0.20f;

// Init Reverb Taps 

  int tcombl[16] =
    { 10586, 12340, 6400, 13100, 8004, 7200, 5130, 9037, 12045, 11237, 9456,
    7634, 5389, 8056, 10120, 11432
  };
  int tcombr[16] =
    { 10518, 11340, 8450, 11100, 9644, 7560, 9536, 11507, 12600, 11111, 8056,
    6048, 7690, 5978, 8845, 10056
  };
  int tapsg[16] =
    { 36, 33, 29, 27, 24, 21, 17, 15, 13, 16, 21, 24, 27, 31, 33, 36 };

  int i;

  for (i = 0; i < 16; i++)

    {
      s->a_combl[i] = 0;
      s->a_combr[i] = 0;
      s->combl[i] = tcombl[i] / 8;
      s->combr[i] = tcombr[i] / 8;
      s->apsg[i] = tapsg[i];
      s->apss += s->apsg[i];
    }

  for (i = 0; i < 16; i++)
    s->ready_apsg[i] = (float) s->apsg[i] / (float) s->apss;


// Init Bank

  New_Bank (s);


  // Init gated notes

  for (i = 0; i < POLY; i++)
    {
      s->note_active[i] = 0;
      s->env_time[i] = 0;
      s->dcphi[i] = 0.0;
    }

  // Init frequency Notes 


  for (i = 1; i <= 192; i++)
    {
      s->h[i].f1 = 8.1757989156 * expf ((float) (i - 2) * logf (2.0f) / 12.0f);
      s->h[i].f2 = 8.1757989156 * expf ((float) (i) * logf (2.0f) / 12.0f);
      s->h[i].f3 = 8.1757989156 * expf ((float) (i + 2) * logf (2.0f) / 12.0f);
    }

// Init waves


  size_t sizesin = (size_t) (D_PI * 1000) + 2;


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
      s->msin[i] = sinf (x_sin + sinf (1.5f * x_sin));
      if (i > 0)
	s->msin[i - 1] =
	  (s->msin[i - 1] * (1.0f + s->msin[i] - s->msin[i - 1]));
      if (i > 1)
	s->msin[i - 2] =
	  (s->msin[i - 2] * (1.0f + s->msin[i - 1] - s->msin[i - 2]));
      if (i > 2)
	s->msin[i - 3] =
	  (s->msin[i - 3] * (1.0f + s->msin[i - 2] - s->msin[i - 3]));
      if (i > 3)
	s->msin[i - 4] =
	  (s->msin[i - 4] * (1.0f + s->msin[i - 3] - s->msin[i - 4]));
      if (i > 4)
	s->msin[i - 5] =
	  (s->msin[i - 5] * (1.0f + s->msin[i - 4] - s->msin[i - 5]));
      if (i > 5)
	s->msin[i - 6] =
	  (s->msin[i - 6] * (1.0f + s->msin[i - 5] - s->msin[i - 6]));
      if (i > 6)
	s->msin[i - 7] =
	  (s->msin[i - 7] * (1.0f + s->msin[i - 6] - s->msin[i - 7]));
      if (i > 7)
	s->msin[i - 8] =
	  (s->msin[i - 8] * (1.0f + s->msin[i - 7] - s->msin[i - 8]));

      s->psin[i] = sinf (x_sin + sinf (2.0f * x_sin));
      s->qsin[i] = sinf (s->nsin[i] + s->lsin[i] + s->psin[i]);
      s->rsin[i] = sinf (s->lsin[i] + sinf (s->msin[i]));
      s->tsin[i] = sinf (s->lsin[i]);

      s->ssin[i] =
	(s->lsin[i] + s->nsin[i] + s->msin[i] + s->psin[i] + s->qsin[i] +
	 s->rsin[i] + s->tsin[i]) / 7.0f;
      if (i > 0)
	s->ssin[i - 1] =
	  (s->ssin[i - 1] * (1.0f + s->ssin[i] - s->ssin[i - 1]));
      if (i > 1)
	s->ssin[i - 2] =
	  (s->ssin[i - 2] * (1.0f + s->ssin[i - 1] - s->ssin[i - 2]));
      if (i > 2)
	s->ssin[i - 3] =
	  (s->ssin[i - 3] * (1.0f + s->ssin[i - 2] - s->ssin[i - 3]));
      if (i > 3)
	s->ssin[i - 4] =
	  (s->ssin[i - 4] * (1.0f + s->ssin[i - 3] - s->ssin[i - 4]));
      if (i > 4)
	s->ssin[i - 5] =
	  (s->ssin[i - 5] * (1.0f + s->ssin[i - 4] - s->ssin[i - 5]));
      if (i > 5)
	s->ssin[i - 6] =
	  (s->ssin[i - 6] * (1.0f + s->ssin[i - 5] - s->ssin[i - 6]));
      if (i > 6)
	s->ssin[i - 7] =
	  (s->ssin[i - 7] * (1.0f + s->ssin[i - 6] - s->ssin[i - 7]));
      if (i > 7)
	s->ssin[i - 8] =
	  (s->ssin[i - 8] * (1.0f + s->ssin[i - 7] - s->ssin[i - 8]));

      s->usin[i] = sinf (s->ssin[i]);

    }


  for (i = 0; i < (int) sizesin; i++)
    {
      x_sin = (float) (i * D_PI / sizesin);
      s->ssin[i] = sinf (x_sin + s->usin[i]);

    }


// Init Buffers

  Clean_Buffer_Effects(s);


};




void
Adjust_Audio (horgand_synth_t * s)
{
  s->increment = .5f / (float) s->SAMPLE_RATE;
  s->D_PI_to_SAMPLE_RATE = D_PI / (float) s->SAMPLE_RATE;
}

// Keyboard Level Scaling
float
Get_Keyb_Level_Scaling (horgand_synth_t * s, int nota)
{
  return (1.5f * s->velocity[nota] * sin ((120 - s->note[nota]) / 120.0f));
};




// Returns the FM Operator Pitch (Frequency + LFO)

float
pitch_Operator (horgand_synth_t * s, int i, int note)
{
  return (s->lasfreq[s->a.Operator[i].harmonic] +
	  s->a.Operator[i].harmonic_fine);
}


float
pitch_Operator2 (horgand_synth_t * s, int i, int note)
{
  return (s->lasfreq[s->a.Operator[i].harmonic] -
	  s->a.Operator[i].harmonic_fine);
}




// Returns The FM Operator Volume 

void
volume_Operator (horgand_synth_t * s, int i, int l2)
{
  s->a.Operator[i].con1 =
    s->a.Operator[i].volumen * s->velocity[l2] *
    s->a.Normalize[s->a.Operator[i].harmonic];

};



// Turn Off all the sound


void
panic (horgand_synth_t * s)
{
  int i;
  for (i = 0; i < POLY; i++)
    {
      s->note_active[i] = 0;
      s->env_time[i] = 0.0f;
      s->gate[i] = 0;
    }

   Clean_Buffer_Effects (s);
   memset (s->f, 0, sizeof s->f);
};



float
Cenvelope (horgand_synth_t * s, int *note_active, int gate, float t, int nota)
{

  float val = 0;

  if (t > s->a.c_attack + s->a.c_decay)
    return 0.0f;
  if (t > s->a.c_attack)
    val = 1.0f - (t - s->a.c_attack) * s->u_c_decay;
  else
    val = t * s->u_c_attack;

  if (gate)
    return (s->a.Click_Vol * val);
  return (s->a.Click2_Vol * val);
};




float
Penvelope (horgand_synth_t * s, int *note_active, int gate, float t, int nota)
{

  if ((gate) || (s->pedal))
    {
      if (t > s->a.p_attack + s->a.p_decay)
	return 0.0f;
      if (t > s->a.p_attack)
	return (1.0f - (t - s->a.p_attack) * s->u_p_decay);
      return (t * s->u_p_attack);
    }
  else
    return s->Perc_Volume[nota] * (1.0f - t * s->u_p_release);




};



float
Jenvelope (horgand_synth_t * s, int *note_active, int gate, float t, int nota)
{

  float Env = 0.0f;

  if (gate)
    {
      if (t > s->a.attack + s->a.decay)
	return (s->a.sustain);
      if (t > s->a.attack)
	return (1.0f -
		(1.0f - s->a.sustain) * (t - s->a.attack) * s->u_decay);

      return (t * s->u_attack);
    }
  else
    {

      if (s->pedal == 0)
	{
	  if (s->release > t)
	    {
	      Env = s->Envelope_Volume[nota] * (1.0f - t * s->u_release);
	      if (Env < 0.0015f)
		{
		  if (*note_active)
		    *note_active = 0;
		  s->env_time[nota] = 0.0f;
		  return (0.0f);
		}

	      else
		return (Env);
	    }
	  else
	    {
	      if (*note_active)
		*note_active = 0;
	      s->env_time[nota] = 0.0f;
	      return (0.0f);
	    }
	}

      else
	{
	  if (s->a.sustain != 0)
	    return (s->a.sustain);
	  else
	    return (1.0f - (t - s->a.attack) * s->u_decay);
	}
    }

  return (Env);
};



// Returns Pitch LFO 


float
Pitch_LFO (horgand_synth_t * s, float t)
{

  float x, out;

  if (t * 20 < s->a.Pitch_LFO_Delay)
    return (0.0f);

  x = fmodf (s->a.Pitch_LFO_Speed * t, 1.0f);

  out = NFsin (s, s->a.LFO_Wave, x * D_PI) * s->LFO_Frequency;

  return (out);

}


// Return Played Note Frequency

float
Get_Partial (horgand_synth_t * s, int nota)
{
  int l;
  float partial = 0;
  float freq_note = 0;

  l = s->note[nota] + s->transpose + s->a.organ_transpose;
  freq_note =
    (s->pitch >
     0) ? s->h[l].f2 + (s->h[l].f3 - s->h[l].f2) * s->pitch : s->h[l].f2 +
    (s->h[l].f2 - s->h[l].f1) * s->pitch;
  partial = (1.0f + *(s->tune) / 16.0f) * freq_note * s->D_PI_to_SAMPLE_RATE;
  if (partial > D_PI)
    partial = fmodf (partial, D_PI);
  return (partial);


};


void
Calc_LFO_Frequency (horgand_synth_t * s)
{

  s->LFO_Frequency =
    s->a.modulation * s->a.LFOpitch * s->D_PI_to_SAMPLE_RATE*.5f;

};




float
NFsin (horgand_synth_t * s, int i, float x)
{


  long int k = F2I (x * 1000.0f);


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
};




// Main Audio thread

void
Alg1s (horgand_synth_t * s, int nframes)
{

  int l2, i;
  int l1;
  float sound, sound2;
  float Env_Vol = 0.0f;
  float Am_Click = 0.0f;
  float Click_TVol = 0.0f;
  float Click_Env = 0.0f;
  float m_partial;

  memset (s->bufl, 0, sizeof(float) * nframes);
  memset (s->bufr, 0, sizeof(float) * nframes);
    
 
  for (l2 = 0; l2 < POLY; l2++)
    {

      if (s->note_active[l2])
	{

	  m_partial = Get_Partial (s, l2);
	  for (i = 1; i <= 10; i++)
	    volume_Operator (s, i, l2);

	  for (l1 = 0; l1 < nframes; l1++)
	    {

	      sound = 0.0f;
	      sound2 = 0.0f;
	      s->Envelope_Volume[l2] =
		Jenvelope (s, &s->note_active[l2], s->gate[l2],
			   s->env_time[l2], l2);
	      s->Perc_Volume[l2] =
		Penvelope (s, &s->note_active[l2], s->gate[l2],
			   s->env_time[l2], l2);
	      s->LFO_Volume = Pitch_LFO (s, s->env_time[l2]);

	      if (s->a.Click)
		{
		  Click_Env =
		    Cenvelope (s, &s->note_active[l2], s->gate[l2],
			       s->env_time[l2], l2);
		  if (Click_Env > 0.0f)
		    {
		      s->dcphi[l2] += s->Click_sFreq;
		      s->dcphi2[l2] += s->Click_2sFreq;
		      if (s->dcphi[l2] > D_PI)
			s->dcphi[l2] -= D_PI;
		      if (s->dcphi2[l2] > D_PI)
			s->dcphi2[l2] -= D_PI;
		      Click_TVol =
			Click_Env * s->velocity[l2] * s->organ_master;
		      Am_Click =
			s->a.Click_Vol1 * Click_TVol * NFsin (s, 3,
								 s->dcphi
								 [l2]);
		      Am_Click +=
			s->a.Click_Vol2 * Click_TVol * NFsin (s, 3,
								 s->dcphi2
								 [l2]);
		      s->bufl[l1] += Am_Click;
		      s->bufr[l1] += Am_Click;
		    }
		}
		
	      for (i = 1; i <=10; i++)
		{

		  if (s->a.Operator[i].marimba == 0)
		    Env_Vol =
		      s->Envelope_Volume[l2] * s->a.Operator[i].con1;
		  else
		    Env_Vol = s->Perc_Volume[l2] * s->a.Operator[i].con1;

		  if (Env_Vol > 0.0f)
		    {

		      s->f[i].dphi = m_partial * (s->p_op[i] + s->LFO_Volume);
		      if (s->f[i].dphi > D_PI)
			s->f[i].dphi -= D_PI;
		      s->f[i].phi[l2] += s->f[i].dphi;
		      if (s->f[i].phi[l2] > D_PI)
			s->f[i].phi[l2] -= D_PI;

		      s->f[i].dphi2 = m_partial * (s->p_op2[i] + s->LFO_Volume);
		      if (s->f[i].dphi2 > D_PI)
			s->f[i].dphi2 -= D_PI;
		      s->f[i].phi2[l2] += s->f[i].dphi2;
		      if (s->f[i].phi2[l2] > D_PI)
			s->f[i].phi2[l2] -= D_PI;

		      sound += Env_Vol * NFsin (s, s->k[i], s->f[i].phi[l2]);
		      sound2 += Env_Vol * NFsin (s, s->k[i], s->f[i].phi2[l2]);
		    }
		}

	      s->bufl[l1] += sound * s->organ_master;
	      s->bufr[l1] += sound2 * s->organ_master;
	      s->env_time[l2] += s->increment;
	    }

	}

    }



  if (s->a.E_Chorus_On)
    Effect_Chorus (s, nframes);
  if (s->a.E_Rotary_On)
    Effect_Rotary (s, nframes);
  if (s->a.E_Delay_On)
    Effect_Delay (s, nframes);
  if (s->a.E_Reverb_On)
    Effect_Reverb (s, nframes);
  Write_Buffer_Effects (s, nframes);


};

