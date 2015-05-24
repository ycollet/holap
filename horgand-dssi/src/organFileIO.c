/*
  horgand.dssi - a synth-organ plugin software

 organ.FileIO.c  -  File Input/Output functions
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

#define BANK_EXTENSION ".horeb"

int
loadbank (horgand_synth_t * s, const char *filename)
{

  int i, j;
  FILE *fn;
  char buf[2048];

  if ((fn = fopen (filename, "r")) == NULL)
    {
      fprintf (stderr, "File not found\n");
      return 1;
    }

  bzero (buf, sizeof (buf));
  fgets (buf, sizeof buf, fn);


  New_Bank (s);

  for (j = 1; j <= 32; j++)
    {
      for (i = 1; i <= 10; i++)
	{
	  bzero (buf, sizeof (buf));
	  fgets (buf, sizeof buf, fn);

	  sscanf (buf, "%d,%f,%f,%d",
		  &s->Banco[j].Operator[i].harmonic,
		  &s->Banco[j].Operator[i].harmonic_fine,
		  &s->Banco[j].Operator[i].volumen,
		  &s->Banco[j].Operator[i].marimba);
	}

      bzero (buf, sizeof (buf));
      fgets (buf, sizeof buf, fn);
      sscanf (buf,
	      "%f,%d,%f,%f,%f,%d,%f,%f", &s->Banco[j].Organ_Master_Volume,
	      &s->Banco[j].organ_transpose, &s->Banco[j].Pitch_LFO_Speed,
	      &s->Banco[j].Pitch_LFO_Delay, &s->Banco[j].Rotary_LFO_Speed,
	      &s->Banco[j].E_Rotary_On, &s->Banco[j].LFOpitch,
	      &s->Banco[j].modulation);

      bzero (buf, sizeof (buf));
      fgets (buf, sizeof buf, fn);
      sscanf (buf,
	      "%f,%d,%f,%d,%f,%f", &s->Banco[j].attack,
	      &s->Banco[j].E_Reverb_On, &s->Banco[j].detune,
	      &s->Banco[j].E_Delay_On, &s->Banco[j].Delay_Delay,
	      &s->Banco[j].Delay_Volume);

      bzero (buf, sizeof (buf));
      fgets (buf, sizeof buf, fn);
      sscanf
	(buf, "%d,%d,%f,%d,%f,%f,%f\n", &s->Banco[j].scaling,
	 &s->Banco[j].E_Chorus_On, &s->Banco[j].Chorus_Delay,
	 &s->Banco[j].Reverb_Preset, &s->Banco[j].Chorus_LFO_Speed,
	 &s->Banco[j].Chorus_LFO_Amplitude, &s->Banco[j].Chorus_Volume);


      bzero (buf, sizeof (buf));
      fgets (buf, sizeof buf, fn);
      sscanf (buf, "%f,%f,%f,%f,%f\n", &s->Banco[j].decay,
	      &s->Banco[j].sustain, &s->Banco[j].p_attack,
	      &s->Banco[j].p_decay, &s->Banco[j].Rotary_LFO_Amplitude);
      for (i = 1; i <= 22; i++)
	{
	  bzero (buf, sizeof (buf));
	  fgets (buf, sizeof buf, fn);
	  sscanf (buf, "%f\n", &s->Banco[j].Normalize[i]);
	}

	  bzero (buf, sizeof (buf));
	  fgets (buf, sizeof buf, fn);
	  sscanf (buf, "%d,%d,%f,%f,%f,%f,%f,%f\n", &s->Banco[j].Speed_Sync,
		  &s->Banco[j].Click, &s->Banco[j].Click_Vol,
		  &s->Banco[j].Click2_Vol, &s->Banco[j].Click_Freq,
		  &s->Banco[j].Click_Freq2, &s->Banco[j].Click_Vol1,
		  &s->Banco[j].Click_Vol2);


	  for (i = 1; i <= 10; i++)
	    {
	      bzero (buf, sizeof (buf));
	      fgets (buf, sizeof buf, fn);
	      sscanf (buf, "%d\n", &s->Banco[j].Operator[i].wave);
	    }


	  bzero (buf, sizeof (buf));
	  fgets (buf, sizeof buf, fn);
	  sscanf (buf, "%d,%d,%d\n", &s->Banco[j].LFO_Wave,
		  &s->Banco[j].Chorus_Wave, &s->Banco[j].Rotary_Wave);



      bzero (buf, sizeof (buf));
      bzero (s->Banco[j].Name, sizeof (s->Banco[j].Name));
      fgets (buf, sizeof buf, fn);
      for (i = 0; i <= 24; i++)
	if (buf[i] > 20)
	  s->Banco[j].Name[i] = buf[i];
      s->Banco[j].nombre = s->Banco[j].Name;
      bzero (buf, sizeof (buf));
      fgets (buf, sizeof buf, fn);

    }

  fclose (fn);
  return 0;
};
