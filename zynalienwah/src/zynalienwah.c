/*
  zynalienwah.c - ZynAddSubFx Alienwah LADSPA plugin software

  zynalienwah.c  -  Main LADSPA file of the plugin
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

rdff19 ZynAlienwah_DRY 4
rdff19 ZynAlienwah_WET 5
rdff19 ZynAlienwah_PAN 6
rdff19 ZynAlienwah_DEPTH 7
rdff19 ZynAlienwah_DELAY 8
rdff19 ZynAlienwah_FB 9
rdff19 ZynAlienwah_LRCROSS 10
rdff19 ZynAlienwah_RANDOM 11
rdff19 ZynAlienwah_LFOTYPE 12
rdff19 ZynAlienwah_STEREO 13
rdff19 ZynAlienwah_LFOFREQ 14
rdff19 ZynAlienwah_PHASE 15


*/


#include <stdio.h>
#include <stdlib.h>
#include <ladspa.h>
#include "alienwah.h"


#define ZynAlienwah_INPUT_L 0
#define ZynAlienwah_INPUT_R 1
#define ZynAlienwah_OUTPUT_L 2
#define ZynAlienwah_OUTPUT_R 3
#define ZynAlienwah_DRY 4
#define ZynAlienwah_WET 5
#define ZynAlienwah_PAN 6
#define ZynAlienwah_DEPTH 7
#define ZynAlienwah_DELAY 8
#define ZynAlienwah_FB 9
#define ZynAlienwah_LRCROSS 10
#define ZynAlienwah_RANDOM 11
#define ZynAlienwah_LFOTYPE 12
#define ZynAlienwah_STEREO 13
#define ZynAlienwah_LFOFREQ 14
#define ZynAlienwah_PHASE 15

static LADSPA_Descriptor *ZynAlienwahLDescriptor = NULL;

static void runZynAlienwah (LADSPA_Handle instance, unsigned long sample_count);

static void cleanupZynAlienwah (LADSPA_Handle instance);


const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return ZynAlienwahLDescriptor;
    default:
      return NULL;
    }
}


static void
cleanupZynAlienwah (LADSPA_Handle instance)
{
  ZAlienwah_t *alienwah = (ZAlienwah_t *) instance;
  free (alienwah);
}

static void
connectPortZynAlienwah (LADSPA_Handle instance, unsigned long port,
		    LADSPA_Data * data)
{
  ZAlienwah_t *plugin = (ZAlienwah_t *) instance;
  switch (port)
    {
    case ZynAlienwah_INPUT_L:
      plugin->inputl = data;
      break;
    case ZynAlienwah_INPUT_R:
      plugin->inputr = data;
      break;
    case ZynAlienwah_OUTPUT_L:
      plugin->efxoutl = data;
      break;
    case ZynAlienwah_OUTPUT_R:
      plugin->efxoutr = data;
      break;
    case ZynAlienwah_DRY:
      plugin->dry = data;
      break;
    case ZynAlienwah_WET:
      plugin->outvolume = data;
      break;
    case ZynAlienwah_PAN:
      plugin->panning = data;
      break;
    case ZynAlienwah_DEPTH:
      plugin->depth = data;
      break;
    case ZynAlienwah_DELAY:
      plugin->delay = data;
      break;
    case ZynAlienwah_FB:
      plugin->fb = data;
      break;
    case ZynAlienwah_LRCROSS:
      plugin->lrcross = data;
      break;
    case ZynAlienwah_RANDOM:
      plugin->lfornd = data;
      break;
    case ZynAlienwah_LFOTYPE:
      plugin->PLFOtype = data;
      break;
    case ZynAlienwah_STEREO:
      plugin->Pstereo = data;
      break;
    case ZynAlienwah_LFOFREQ:
      plugin->lfofreq = data;
      break;
    case ZynAlienwah_PHASE:
      plugin->Phase = data;
      break;    
    }
}



static LADSPA_Handle
instantiateZynAlienwah (const LADSPA_Descriptor * descriptor,
		    unsigned long s_rate)
{



  ZAlienwah_t *alienwah = (ZAlienwah_t *) calloc (1, sizeof (ZAlienwah_t));
  
  if (alienwah)
  {
  alienwah->SAMPLE_RATE = s_rate;
  Alienwah_Init (alienwah);
  EffectLFO_Init(alienwah); 
  }

  return (LADSPA_Handle) alienwah;
}

static void
activateZynAlienwah (LADSPA_Handle instance)
{

//  ZAlienwah_t *alienwah = (ZAlienwah_t *) instance;


}


void
ZynAlienwah_deactivate (LADSPA_Handle instance)
{

//  ZAlienwah_t *alienwah = (ZAlienwah_t *) instance;


}


static void
runZynAlienwahWrapper (LADSPA_Handle instance, unsigned long sample_count)
{
  runZynAlienwah (instance, sample_count);

}

static void
runZynAlienwah (LADSPA_Handle instance, unsigned long sample_count)
{

  ZAlienwah_t *alienwah = (ZAlienwah_t *) instance;

  LADSPA_Data *const outputl = alienwah->efxoutl;
  LADSPA_Data *const outputr = alienwah->efxoutr;
  LADSPA_Data *const pinputl = alienwah->inputl;
  LADSPA_Data *const pinputr = alienwah->inputr;
  LADSPA_Data dry = *(alienwah->dry);


  int i;

   for (i=0; i<sample_count; i++)
        {
          alienwah->indatal[i]=pinputl[i];
          alienwah->indatar[i]=pinputr[i];
        }

    out(alienwah, pinputl,pinputr,sample_count);
 

        for (i=0; i<sample_count; i++)

        {
          outputl[i] += alienwah->indatal[i]*dry;
          outputr[i] += alienwah->indatar[i]*dry;
        }
  

}


void __attribute__((constructor))
zynalienwah_init()

{


  char **port_names;
  LADSPA_PortDescriptor *port_descriptors;
  LADSPA_PortRangeHint *port_range_hints;
  ZynAlienwahLDescriptor =
    (LADSPA_Descriptor *) malloc (sizeof (LADSPA_Descriptor));
  if (ZynAlienwahLDescriptor)
    {
      ZynAlienwahLDescriptor->UniqueID = 3984;
      ZynAlienwahLDescriptor->Label = "ZynAlienwah";
      ZynAlienwahLDescriptor->Properties =  
      LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
      ZynAlienwahLDescriptor->Name = "ZynAlienwah";
      ZynAlienwahLDescriptor->Maker = "Josep Andreu <holborn@telefonica.net>";
      ZynAlienwahLDescriptor->Copyright = "GNU General Public License version 2";
      ZynAlienwahLDescriptor->PortCount = 16;
      port_descriptors =
	(LADSPA_PortDescriptor *)
	calloc (ZynAlienwahLDescriptor->PortCount,
		sizeof (LADSPA_PortDescriptor));
      ZynAlienwahLDescriptor->PortDescriptors =
	(const LADSPA_PortDescriptor *) port_descriptors;
      port_range_hints =
	(LADSPA_PortRangeHint *) calloc (ZynAlienwahLDescriptor->PortCount,
					 sizeof (LADSPA_PortRangeHint));
      ZynAlienwahLDescriptor->PortRangeHints =
	(const LADSPA_PortRangeHint *) port_range_hints;
      port_names =
	(char **) calloc (ZynAlienwahLDescriptor->PortCount, sizeof (char *));
      ZynAlienwahLDescriptor->PortNames = (const char **) port_names;

      /* Parameters for Input L */
      port_descriptors[ZynAlienwah_INPUT_L] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[ZynAlienwah_INPUT_L] = "Input L";
      port_range_hints[ZynAlienwah_INPUT_L].HintDescriptor = 0;

      /* Parameters for Input R */
      port_descriptors[ZynAlienwah_INPUT_R] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[ZynAlienwah_INPUT_R] = "Input R";
      port_range_hints[ZynAlienwah_INPUT_R].HintDescriptor = 0;

      /* Parameters for Output L */
      port_descriptors[ZynAlienwah_OUTPUT_L] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[ZynAlienwah_OUTPUT_L] = "Output L";
      port_range_hints[ZynAlienwah_OUTPUT_L].HintDescriptor = 0;

      /* Parameters for Output R */
      port_descriptors[ZynAlienwah_OUTPUT_R] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[ZynAlienwah_OUTPUT_R] = "Output R";
      port_range_hints[ZynAlienwah_OUTPUT_R].HintDescriptor = 0;

      /* Parameters for Dry */
      port_descriptors[ZynAlienwah_DRY] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynAlienwah_DRY] = "Dry";
      port_range_hints[ZynAlienwah_DRY].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE; 
      port_range_hints[ZynAlienwah_DRY].LowerBound = 0.0;
      port_range_hints[ZynAlienwah_DRY].UpperBound = 2.0;

      /* Parameters for Volume */
      port_descriptors[ZynAlienwah_WET] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynAlienwah_WET] = "Wet";
      port_range_hints[ZynAlienwah_WET].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynAlienwah_WET].LowerBound = 0.0;
      port_range_hints[ZynAlienwah_WET].UpperBound = 2.0;

      /* Parameters for Pan */
      port_descriptors[ZynAlienwah_PAN] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynAlienwah_PAN] = "Pan";
      port_range_hints[ZynAlienwah_PAN].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynAlienwah_PAN].LowerBound = 0.0;
      port_range_hints[ZynAlienwah_PAN].UpperBound = 1.0;

      /* Parameters for Depth */
      port_descriptors[ZynAlienwah_DEPTH] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynAlienwah_DEPTH] = "Depth";
      port_range_hints[ZynAlienwah_DEPTH].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MINIMUM;
      port_range_hints[ZynAlienwah_DEPTH].LowerBound = 0.0;
      port_range_hints[ZynAlienwah_DEPTH].UpperBound = 1.0;

      /* Parameters for Delay */
      port_descriptors[ZynAlienwah_DELAY] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynAlienwah_DELAY] = "Delay";
      port_range_hints[ZynAlienwah_DELAY].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MINIMUM;
      port_range_hints[ZynAlienwah_DELAY].LowerBound = 0.0;
      port_range_hints[ZynAlienwah_DELAY].UpperBound = 1.0;

      /* Parameters for FB */
      port_descriptors[ZynAlienwah_FB] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynAlienwah_FB] = "Feedback";
      port_range_hints[ZynAlienwah_FB].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynAlienwah_FB].LowerBound = -0.99;
      port_range_hints[ZynAlienwah_FB].UpperBound = 0.99;

      /* Parameters for LRCROSS */
      port_descriptors[ZynAlienwah_LRCROSS] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynAlienwah_LRCROSS] = "LR Cross";
      port_range_hints[ZynAlienwah_LRCROSS].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynAlienwah_LRCROSS].LowerBound = 0.0;
      port_range_hints[ZynAlienwah_LRCROSS].UpperBound = 1.0;

      /* Parameters for RANDOM */
      port_descriptors[ZynAlienwah_RANDOM] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynAlienwah_RANDOM] = "Randomness";
      port_range_hints[ZynAlienwah_RANDOM].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynAlienwah_RANDOM].LowerBound = 0.0;
      port_range_hints[ZynAlienwah_RANDOM].UpperBound = 1.0;

      /* Parameters for LFOTYPE */
      port_descriptors[ZynAlienwah_LFOTYPE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynAlienwah_LFOTYPE] = "Type LFO";
      port_range_hints[ZynAlienwah_LFOTYPE].HintDescriptor =
	LADSPA_HINT_DEFAULT_0 | LADSPA_HINT_TOGGLED;
      port_range_hints[ZynAlienwah_LFOTYPE].LowerBound = 0;
      port_range_hints[ZynAlienwah_LFOTYPE].UpperBound = 1;

      /* Parameters for STEREO */
      port_descriptors[ZynAlienwah_STEREO] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynAlienwah_STEREO] = "Stereo df";
      port_range_hints[ZynAlienwah_STEREO].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynAlienwah_STEREO].LowerBound = -0.5;
      port_range_hints[ZynAlienwah_STEREO].UpperBound = 0.5;


      /* Parameters for LFOFREQ */
      port_descriptors[ZynAlienwah_LFOFREQ] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynAlienwah_LFOFREQ] = "LFO Frequency";
      port_range_hints[ZynAlienwah_LFOFREQ].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynAlienwah_LFOFREQ].LowerBound = 0.0;
      port_range_hints[ZynAlienwah_LFOFREQ].UpperBound = 1.0;

      
      /* Parameters for PHASE */
      port_descriptors[ZynAlienwah_PHASE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynAlienwah_PHASE] = "Phase";
      port_range_hints[ZynAlienwah_PHASE].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE | 
       	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynAlienwah_PHASE].LowerBound = -1.0;
      port_range_hints[ZynAlienwah_PHASE].UpperBound = 1.0;

      
      ZynAlienwahLDescriptor->instantiate = instantiateZynAlienwah;
      ZynAlienwahLDescriptor->connect_port = connectPortZynAlienwah;
      ZynAlienwahLDescriptor->activate = activateZynAlienwah;
      ZynAlienwahLDescriptor->run = runZynAlienwahWrapper;
      ZynAlienwahLDescriptor->run_adding = NULL;
      ZynAlienwahLDescriptor->set_run_adding_gain = NULL;
      ZynAlienwahLDescriptor->deactivate = ZynAlienwah_deactivate;
      ZynAlienwahLDescriptor->cleanup = cleanupZynAlienwah;
    }



}


void __attribute((destructor))
zynalienwah_fini()

{
  if (ZynAlienwahLDescriptor)
    {
      free ((LADSPA_PortDescriptor *) ZynAlienwahLDescriptor->PortDescriptors);
      free ((char **) ZynAlienwahLDescriptor->PortNames);
      free ((LADSPA_PortRangeHint *) ZynAlienwahLDescriptor->PortRangeHints);
      free (ZynAlienwahLDescriptor);
    }
}



