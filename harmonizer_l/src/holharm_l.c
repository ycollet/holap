/*
  holharm - a inteligent harmonizer DSSI plugin software

  holharm.c  -  Main dssi file of the plugin
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



#include <math.h>
#include <ladspa.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "harmonizer.h"


#define holharm_INPUT_L 0
#define holharm_INPUT_R 1
#define holharm_OUTPUT_L 2
#define holharm_OUTPUT_R 3
#define holharm_DRY 4
#define holharm_VOLUME 5
#define holharm_PAN 6
#define holharm_INTERVALL 7
#define holharm_INTERVALR 8
#define holharm_TYPE 9
#define holharm_NOTE 10
#define holharm_CHORDTYPE 11
#define holharm_RECOCOUNT 12

static LADSPA_Descriptor *holharm_lLDescriptor = NULL;

static void runholharm_l (LADSPA_Handle instance, unsigned long sample_count);

static void cleanupholharm_l(LADSPA_Handle instance);


const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return holharm_lLDescriptor;
    default:
      return NULL;
    }
}


static void
cleanupholharm_l (LADSPA_Handle instance)
{
  holharm_t *harmonizer = (holharm_t *) instance;
  free (harmonizer);
}

static void
connectPortholharm_l(LADSPA_Handle instance, unsigned long port,
		    LADSPA_Data * data)
{
  holharm_t *plugin = (holharm_t *) instance;
  switch (port)
    {
    case holharm_INPUT_L:
      plugin->inputl = data;
      break;
    case holharm_INPUT_R:
      plugin->inputr = data;
      break;
    case holharm_OUTPUT_L:
      plugin->efxoutl = data;
      break;
    case holharm_OUTPUT_R:
      plugin->efxoutr = data;
      break;
    case holharm_DRY:
      plugin->dry = data;
      break;
    case holharm_VOLUME:
      plugin->outvolume = data;
      break;
    case holharm_PAN:
      plugin->panning = data;
      break;
    case holharm_INTERVALL:
      plugin->intervall = data;
      break;
    case holharm_INTERVALR:
      plugin->intervalr = data;
      break;
    case holharm_TYPE:
      plugin->PSELECT = data;
      break;
    case holharm_NOTE:
      plugin->Hnote = data;
      break;
    case holharm_CHORDTYPE:
      plugin->Ptype = data;
      break;
    case holharm_RECOCOUNT:
      plugin->recocount = data;
      break;  
    }
}



int
ReadConfig ()
{

  FILE *fs;
  int Quality;
  char temp[32];
  char filename[128];
  Quality = QUALITY;
  sprintf (filename, "/home/%s/.holharm", getenv("USER"));
  if ((fs = fopen (filename, "r")) != NULL)
    {
      bzero (temp, sizeof (temp));
      fgets (temp, sizeof temp, fs);
      sscanf (temp, "%d", &Quality);
      fclose (fs);
    }
  else
    {
      fs = fopen (filename, "w");
      bzero (temp, sizeof (temp));
      sprintf (temp, "%d\n", Quality);
      fputs (temp, fs);
      fclose (fs);
    }
  return (int)Quality;

}

static LADSPA_Handle
instantiateholharm_l(const LADSPA_Descriptor * descriptor,
		    unsigned long s_rate)
{

  holharm_t *harmonizer = (holharm_t *) calloc (1, sizeof (holharm_t));
  
  harmonizer->hq = ReadConfig();
  RecChord_init (harmonizer);
  harmonizer->SAMPLE_RATE = s_rate;
  Recognize_init (harmonizer,harmonizer->efxoutl, harmonizer->efxoutr);
  PitchShifter_init (harmonizer, 2048, harmonizer->hq,
		     harmonizer->SAMPLE_RATE);
  Harmonizer_start (harmonizer);
  return (LADSPA_Handle) harmonizer;
}

static void
activateholharm_l (LADSPA_Handle instance)
{
  holharm_t *harmonizer = (holharm_t *) instance;
  harmonizer->rc.reconota=-1;
}


void
holharm_l_deactivate(LADSPA_Handle instance)
{
//  holharm_t *harmonizer = (holharm_t *) instance;
}


static void
runholharmWrapper(LADSPA_Handle instance, unsigned long sample_count)
{
  runholharm_l(instance, sample_count);
}

static void
runholharm_l(LADSPA_Handle instance, unsigned long sample_count)
{

  holharm_t *har = (holharm_t *) instance;
  LADSPA_Data *const outputl = har->efxoutl;
  LADSPA_Data *const outputr = har->efxoutr;
  LADSPA_Data *const pinputl = har->inputl;
  LADSPA_Data *const pinputr = har->inputr;
  LADSPA_Data dry = *(har->dry);
  LADSPA_Data vol = *(har->outvolume);
  LADSPA_Data pan = *(har->panning);
  LADSPA_Data interl = *(har->intervall);
  LADSPA_Data interr = *(har->intervalr);
  LADSPA_Data psel = *(har->PSELECT);
  LADSPA_Data Snote = *(har->Hnote);
  LADSPA_Data Ch_Type = *(har->Ptype);  
  LADSPA_Data recocount = *(har->recocount);

  int i;

  
  if (psel == 1)
   {
     har->rc.fundi = Snote;
     har->rc.ctipo = Ch_Type;
   }
   
    
      if ((psel == 1) && (har->val_sum > -24.0))
        {    
           schmittFloat (har, pinputl, pinputr, sample_count);
	  if ((har->rc.reconota != -1) && (har->rc.reconota != har->last))
	    {
	      har->candidate = har->rc.reconota;
	      if (har->candidate != har->lastcandidate) 
               {
	       har->lcount = 0;
	       har->lastcandidate = har->candidate;
	       }
 	       else har->lcount++; 
               if (har->lcount > recocount)
		{
		  Vamos (har, 1, interl);
		  Vamos (har, 2, interr);
		  har->last = har->rc.reconota;
		  har->lcount = 0;
		}
		
	    }
	}



      out_smb (har, pinputl, pinputr, sample_count);
      
       for (i=0; i < sample_count; i++)
        {
          outputl[i]= har->outil[i]*dry + har->outol[i]*vol*pan;    
          outputr[i]= har->outir[i]*dry + har->outor[i]*vol*(1.0-pan);          
        }  



}


void __attribute__ ((constructor))
holharm_l_init()
{


  char **port_names;
  LADSPA_PortDescriptor *port_descriptors;
  LADSPA_PortRangeHint *port_range_hints;
  holharm_lLDescriptor =
    (LADSPA_Descriptor *) malloc (sizeof (LADSPA_Descriptor));
  if (holharm_lLDescriptor)
    {
      holharm_lLDescriptor->UniqueID = 3990;
      holharm_lLDescriptor->Label = "LADSPA holharm";
      holharm_lLDescriptor->Properties = LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
      holharm_lLDescriptor->Name = "holharm_l";
      holharm_lLDescriptor->Maker = "Josep Andreu <holborn@telefonica.net>";
      holharm_lLDescriptor->Copyright = "GNU General Public License version 2";
      holharm_lLDescriptor->PortCount = 13;
      port_descriptors =
	(LADSPA_PortDescriptor *)
	calloc (holharm_lLDescriptor->PortCount,
		sizeof (LADSPA_PortDescriptor));
      holharm_lLDescriptor->PortDescriptors =
	(const LADSPA_PortDescriptor *) port_descriptors;
      port_range_hints =
	(LADSPA_PortRangeHint *) calloc (holharm_lLDescriptor->PortCount,
					 sizeof (LADSPA_PortRangeHint));
      holharm_lLDescriptor->PortRangeHints =
	(const LADSPA_PortRangeHint *) port_range_hints;
      port_names =
	(char **) calloc (holharm_lLDescriptor->PortCount, sizeof (char *));
      holharm_lLDescriptor->PortNames = (const char **) port_names;
      /* Parameters for Input L */
      port_descriptors[holharm_INPUT_L] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[holharm_INPUT_L] = "Input L";
      port_range_hints[holharm_INPUT_L].HintDescriptor = 0;
      /* Parameters for Input R */
      port_descriptors[holharm_INPUT_R] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[holharm_INPUT_R] = "Input R";
      port_range_hints[holharm_INPUT_R].HintDescriptor = 0;
      /* Parameters for Output L */
      port_descriptors[holharm_OUTPUT_L] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[holharm_OUTPUT_L] = "Output L";
      port_range_hints[holharm_OUTPUT_L].HintDescriptor = 0;
      /* Parameters for Output R */
      port_descriptors[holharm_OUTPUT_R] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[holharm_OUTPUT_R] = "Output R";
      port_range_hints[holharm_OUTPUT_R].HintDescriptor = 0;
      /* Parameters for Dry */
      port_descriptors[holharm_DRY] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_DRY] = "Dry";
      port_range_hints[holharm_DRY].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[holharm_DRY].LowerBound = 0.0;
      port_range_hints[holharm_DRY].UpperBound = 1.0;
      /* Parameters for Volume */
      port_descriptors[holharm_VOLUME] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_VOLUME] = "Volume";
      port_range_hints[holharm_VOLUME].HintDescriptor =
	LADSPA_HINT_DEFAULT_MAXIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[holharm_VOLUME].LowerBound = 0.0;
      port_range_hints[holharm_VOLUME].UpperBound = 1.0;
      /* Parameters for Pan */
      port_descriptors[holharm_PAN] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_PAN] = "Pan";
      port_range_hints[holharm_PAN].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[holharm_PAN].LowerBound = 0.0;
      port_range_hints[holharm_PAN].UpperBound = 1.0;
      /* Parameters for Interval L */
      port_descriptors[holharm_INTERVALL] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_INTERVALL] = "Interval L";
      port_range_hints[holharm_INTERVALL].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE | LADSPA_HINT_INTEGER;
      port_range_hints[holharm_INTERVALL].LowerBound = -12;
      port_range_hints[holharm_INTERVALL].UpperBound = 12;
      /* Parameters for Interval R */
      port_descriptors[holharm_INTERVALR] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_INTERVALR] = "Interval R";
      port_range_hints[holharm_INTERVALR].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE | LADSPA_HINT_INTEGER;
      port_range_hints[holharm_INTERVALR].LowerBound = -12;
      port_range_hints[holharm_INTERVALR].UpperBound = 12;
      /* Parameters for Type */
      port_descriptors[holharm_TYPE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_TYPE] = "Mode";
      port_range_hints[holharm_TYPE].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[holharm_TYPE].LowerBound = 0;
      port_range_hints[holharm_TYPE].UpperBound = 1;
      /* Parameters for Note */
      port_descriptors[holharm_NOTE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_NOTE] = "Chord Note Base";
      port_range_hints[holharm_NOTE].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[holharm_NOTE].LowerBound = 0;
      port_range_hints[holharm_NOTE].UpperBound = 11;
      /* Parameters for TypeChord */
      port_descriptors[holharm_CHORDTYPE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_CHORDTYPE] = "Chord Type";
      port_range_hints[holharm_CHORDTYPE].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;	
      port_range_hints[holharm_CHORDTYPE].LowerBound = 0;
      port_range_hints[holharm_CHORDTYPE].UpperBound = 33;
      /* Parameters for RecoCount */
      port_descriptors[holharm_RECOCOUNT] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[holharm_RECOCOUNT] = "Recognize Times";
      port_range_hints[holharm_RECOCOUNT].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE | LADSPA_HINT_INTEGER |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;	
      port_range_hints[holharm_RECOCOUNT].LowerBound = 0;
      port_range_hints[holharm_RECOCOUNT].UpperBound = 48;
      
      
      
      holharm_lLDescriptor->instantiate = instantiateholharm_l;
      holharm_lLDescriptor->connect_port = connectPortholharm_l;
      holharm_lLDescriptor->activate = activateholharm_l;
      holharm_lLDescriptor->run = runholharmWrapper;
      holharm_lLDescriptor->run_adding = NULL;
      holharm_lLDescriptor->set_run_adding_gain = NULL;
      holharm_lLDescriptor->deactivate = holharm_l_deactivate;
      holharm_lLDescriptor->cleanup = cleanupholharm_l;
    }


}


void __attribute__ ((destructor))
holharm__lfini()
{
  if (holharm_lLDescriptor)
    {
      free ((LADSPA_PortDescriptor *) holharm_lLDescriptor->PortDescriptors);
      free ((char **) holharm_lLDescriptor->PortNames);
      free ((LADSPA_PortRangeHint *) holharm_lLDescriptor->PortRangeHints);
      free (holharm_lLDescriptor);
    }
}
