/*
  horgand-dssi a synth-organ plugin software

 organvarios.c  -  miscelaneous functions
  Copyright (C) 2004-2008 Josep Andreu (Holborn)
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


#include "Holrgan.h"
#include <math.h>


void
New_Bank (horgand_synth_t * s)
{
  int j;

  New (s);
  for (j = 1; j <= 32; j++)
    s->Banco[j] = s->a;

};


// Reset vars to New Preset

void
New (horgand_synth_t * s)
{
  int i;

  for (i = 1; i <= 10; i++)
    {
      s->a.Operator[i].harmonic_fine = 0.0;
      s->a.Operator[i].volumen = 0.0;
      s->a.Operator[i].marimba = 0;
      s->a.Operator[i].wave = 1;
    }

  s->a.Operator[1].harmonic = 1;
  s->a.Operator[2].harmonic = 3;
  s->a.Operator[3].harmonic = 4;
  s->a.Operator[4].harmonic = 5;
  s->a.Operator[5].harmonic = 7;
  s->a.Operator[6].harmonic = 8;
  s->a.Operator[7].harmonic = 11;
  s->a.Operator[8].harmonic = 14;
  s->a.Operator[9].harmonic = 19;
  s->a.Operator[10].harmonic = 22;


  s->a.E_Delay_On = 0;
  s->a.Delay_Delay = 0;
  s->a.Delay_Volume = 0;
  s->a.Pitch_LFO_Speed = 0;
  s->a.Pitch_LFO_Delay = 0;
  s->a.Rotary_LFO_Speed = 0;
  s->a.LFOpitch = 0;
  s->a.modulation = .99f;
  s->a.organ_transpose = 0;
  s->a.E_Rotary_On = 0;
  s->pitch = 0;
  s->pedal = 0;
  s->Master_Volume = 0.70f;
  s->a.Organ_Master_Volume = 0.70f;
  s->a.detune = 0;
  s->a.scaling = 0;
  s->a.Chorus_Volume = 0.60f;
  s->a.E_Chorus_On = 0;
  s->a.Chorus_LFO_Speed = 0;
  s->a.Chorus_LFO_Amplitude = 0;
  s->a.Chorus_Delay = 0;
  s->a.Reverb_Preset = 1;
  s->a.E_Reverb_On = 0;
  bzero (s->a.Name, sizeof (s->a.Name));
  s->a.nombre = s->a.Name;
  s->a.Normalize[1] = 1.0f;
  s->a.Normalize[2] = 1.0f;
  s->a.Normalize[3] = 1.0f;
  s->a.Normalize[4] = 1.0f;
  s->a.Normalize[5] = 1.0f;
  s->a.Normalize[6] = 1.0f;
  s->a.Normalize[7] = 1.0f;
  s->a.Normalize[8] = 1.0f;
  s->a.Normalize[9] = .22f;
  s->a.Normalize[10] = .2f;
  s->a.Normalize[11] = .166f;
  s->a.Normalize[12] = .142f;
  s->a.Normalize[13] = .133f;
  s->a.Normalize[14] = .125f;
  s->a.Normalize[15] = .111f;
  s->a.Normalize[16] = .1f;
  s->a.Normalize[17] = 0.095f;
  s->a.Normalize[18] = 0.090f;
  s->a.Normalize[19] = 0.083f;
  s->a.Normalize[20] = 0.076f;
  s->a.Normalize[21] = 0.071f;
  s->a.Normalize[22] = .066f;
  s->a.attack = 0.003f;
  s->u_attack = 1.0f / s->a.attack;
  s->a.decay = 0.28f;
  s->u_decay = 1.0f / s->a.decay;
  s->a.sustain = 0.8f;
  s->a.p_attack = 0.0002f;
  s->u_p_attack = 1.0f / s->a.p_attack;
  s->a.p_decay = 0.077f;
  s->u_p_decay = 1.0f / s->a.p_decay;
  s->p_release = 0.1f;
  s->u_p_release = 1.0f / s->p_release;
  s->a.c_attack = 0.0001f;
  s->a.c_decay = 0.003f;
  s->u_c_attack = 1.0f / s->a.c_attack;
  s->u_c_decay = 1.0f / s->a.c_decay;
  s->a.Click_Vol = .2f;
  s->a.Click2_Vol = .09f;
  s->a.Click_Vol1 = .23f;
  s->a.Click_Vol2 = .04f;
  s->a.Click_Freq = 360;
  s->a.Click_Freq2 = 6712;
  s->a.Click = 0;
  s->a.Rotary_LFO_Amplitude = 9980;
  s->a.Speed_Sync = 0;
  s->a.LFO_Wave = 1;
  s->a.Chorus_Wave = 1;
  s->a.Rotary_Wave = 1;

}

void
Put_Combi_t (horgand_synth_t * s, int i)
{
  s->a = s->Banco[i];
  PutReverb (s, s->a.Reverb_Preset);
  Calc_LFO_Frequency (s);
  Calc_Chorus_LFO_Frequency (s);
  syncadsr (s);

};


void
syncadsr (horgand_synth_t * s)
{

  int i;

  memset (s->f, 0, sizeof s->f);
  memset (s->dcphi, 0, sizeof s->dcphi);


  s->u_attack = 1.0f / s->a.attack;
  s->u_decay = 1.0f / s->a.decay;
  s->u_p_attack = 1.0f / s->a.p_attack;
  s->u_p_decay = 1.0f / s->a.p_decay;
  s->u_c_attack = 1.0f / s->a.c_attack;
  s->u_c_decay = 1.0f / s->a.c_decay;

  s->total_vol = .1f;


  for (i = 1; i < 11; i++)

    {
      s->p_op[i] = pitch_Operator (s, i, 0);
      s->p_op2[i] = pitch_Operator2 (s, i, 0);
      s->total_vol +=
	s->a.Operator[i].volumen *
	s->a.Normalize[s->a.Operator[i].harmonic];
      s->k[i] = s->a.Operator[i].wave;
    }

  s->organ_master = s->a.Organ_Master_Volume / s->total_vol;

  s->Click_sFreq = s->a.Click_Freq * s->D_PI_to_SAMPLE_RATE;
  s->Click_2sFreq = s->a.Click_Freq2 * s->D_PI_to_SAMPLE_RATE;



};



void
PutReverb (horgand_synth_t * s, int i)
{

  int j;

  Clean_Buffer_Effects (s);

  switch (i)
    {
    case 1:
      s->Reverb_Time = 16;
      s->Reverb_Volume = 0.65f;
      s->Reverb_Diffussion = 0.14f;

      break;

    case 2:
      s->Reverb_Time = 18;
      s->Reverb_Volume = 0.65f;
      s->Reverb_Diffussion = 0.14f;
      break;

    case 3:
      s->Reverb_Time = 20;
      s->Reverb_Volume = 0.65f;
      s->Reverb_Diffussion = 0.16f;
      break;

    case 4:
      s->Reverb_Time = 22;
      s->Reverb_Volume = 0.65f;
      s->Reverb_Diffussion = 0.16f;
      break;

    case 5:
      s->Reverb_Time = 24;
      s->Reverb_Volume = 0.65f;
      s->Reverb_Diffussion = 0.16f;
      break;

    case 6:
      s->Reverb_Time = 26;
      s->Reverb_Volume = 0.65f;
      s->Reverb_Diffussion = 0.16f;

    case 7:
      s->Reverb_Time = 30;
      s->Reverb_Volume = 0.65f;
      s->Reverb_Diffussion = 0.16f;
      break;

    case 8:
      s->Reverb_Time = 34;
      s->Reverb_Volume = 0.65f;
      s->Reverb_Diffussion = 0.16f;
      break;
    }

  for (j = 0; j < 16; j++)
    {
      s->a_combl[j] = (s->Reverb_Time * s->combl[j]);
      s->a_combr[j] = (s->Reverb_Time * s->combr[j]);
    }


};
