/*
  zynchorus.c - ZynAddSubFx Chorus LADSPA plugin software

  zynchorus.c  -  Main LADSPA file of the plugin
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

rdff19 ZynChorus_INPUT_L 0
rdff19 ZynChorus_INPUT_R 1
rdff19 ZynChorus_OUTPUT_L 2
rdff19 ZynChorus_OUTPUT_R 3
rdff19 ZynChorus_DRY 4
rdff19 ZynChorus_WET 5
rdff19 ZynChorus_PAN 6
rdff19 ZynChorus_DEPTH 7
rdff19 ZynChorus_DELAY 8
rdff19 ZynChorus_FB 9
rdff19 ZynChorus_LRCROSS 10
rdff19 ZynChorus_RANDOM 11
rdff19 ZynChorus_LFOTYPE 12
rdff19 ZynChorus_STEREO 13
rdff19 ZynChorus_LFOFREQ 14
rdff19 ZynChorus_SUBTRACT 15


*/


#include <stdio.h>
#include <stdlib.h>
#include <ladspa.h>
#include "chorus.h"


#define ZynChorus_INPUT_L 0
#define ZynChorus_INPUT_R 1
#define ZynChorus_OUTPUT_L 2
#define ZynChorus_OUTPUT_R 3
#define ZynChorus_DRY 4
#define ZynChorus_WET 5
#define ZynChorus_PAN 6
#define ZynChorus_DEPTH 7
#define ZynChorus_DELAY 8
#define ZynChorus_FB 9
#define ZynChorus_LRCROSS 10
#define ZynChorus_RANDOM 11
#define ZynChorus_LFOTYPE 12
#define ZynChorus_STEREO 13
#define ZynChorus_LFOFREQ 14
#define ZynChorus_SUBTRACT 15

static LADSPA_Descriptor *ZynChorusLDescriptor = NULL;

static void runZynChorus (LADSPA_Handle instance, unsigned long sample_count);

static void cleanupZynChorus (LADSPA_Handle instance);


const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return ZynChorusLDescriptor;
    default:
      return NULL;
    }
}


static void
cleanupZynChorus (LADSPA_Handle instance)
{
  ZChorus_t *chorus = (ZChorus_t *) instance;
  free (chorus);
}

static void
connectPortZynChorus (LADSPA_Handle instance, unsigned long port,
		    LADSPA_Data * data)
{
  ZChorus_t *plugin = (ZChorus_t *) instance;
  switch (port)
    {
    case ZynChorus_INPUT_L:
      plugin->inputl = data;
      break;
    case ZynChorus_INPUT_R:
      plugin->inputr = data;
      break;
    case ZynChorus_OUTPUT_L:
      plugin->efxoutl = data;
      break;
    case ZynChorus_OUTPUT_R:
      plugin->efxoutr = data;
      break;
    case ZynChorus_DRY:
      plugin->dry = data;
      break;
    case ZynChorus_WET:
      plugin->outvolume = data;
      break;
    case ZynChorus_PAN:
      plugin->panning = data;
      break;
    case ZynChorus_DEPTH:
      plugin->depth = data;
      break;
    case ZynChorus_DELAY:
      plugin->delay = data;
      break;
    case ZynChorus_FB:
      plugin->fb = data;
      break;
    case ZynChorus_LRCROSS:
      plugin->lrcross = data;
      break;
    case ZynChorus_RANDOM:
      plugin->lfornd = data;
      break;
    case ZynChorus_LFOTYPE:
      plugin->PLFOtype = data;
      break;
    case ZynChorus_STEREO:
      plugin->Pstereo = data;
      break;
    case ZynChorus_LFOFREQ:
      plugin->lfofreq = data;
      break;
    case ZynChorus_SUBTRACT:
      plugin->Poutsub = data;
      break;    
    }
}



static LADSPA_Handle
instantiateZynChorus (const LADSPA_Descriptor * descriptor,
		    unsigned long s_rate)
{



  ZChorus_t *chorus = (ZChorus_t *) calloc (1, sizeof (ZChorus_t));
  
  if (chorus)
  {
  chorus->SAMPLE_RATE = s_rate;
  Chorus_Init (chorus);
  EffectLFO_Init(chorus); 
  
  }

  return (LADSPA_Handle) chorus;
}

static void
activateZynChorus (LADSPA_Handle instance)
{

  ZChorus_t *chorus = (ZChorus_t *) instance;


}


void
ZynChorus_deactivate (LADSPA_Handle instance)
{
  ZChorus_t *chorus = (ZChorus_t *) instance;
  Chorus_cleanup (chorus);
}


static void
runZynChorusWrapper (LADSPA_Handle instance, unsigned long sample_count)
{
  runZynChorus (instance, sample_count);

}

static void
runZynChorus (LADSPA_Handle instance, unsigned long sample_count)
{

  ZChorus_t *chorus = (ZChorus_t *) instance;

  LADSPA_Data *const outputl = chorus->efxoutl;
  LADSPA_Data *const outputr = chorus->efxoutr;
  LADSPA_Data *const pinputl = chorus->inputl;
  LADSPA_Data *const pinputr = chorus->inputr;
  LADSPA_Data dry = *(chorus->dry);

  int i;


       for (i=0; i<sample_count; i++)
         {
           chorus->indatal[i]=pinputl[i];
           chorus->indatar[i]=pinputr[i];
         }  

   out(chorus, pinputl,pinputr,sample_count);

          for (i=0; i<sample_count; i++)
           
        {
          outputl[i] +=chorus->indatal[i]*dry;
          outputr[i] +=chorus->indatar[i]*dry;
        }
  


}


void __attribute__((constructor))
zynchorus_init()

{


  char **port_names;
  LADSPA_PortDescriptor *port_descriptors;
  LADSPA_PortRangeHint *port_range_hints;
  ZynChorusLDescriptor =
    (LADSPA_Descriptor *) malloc (sizeof (LADSPA_Descriptor));
  if (ZynChorusLDescriptor)
    {
      ZynChorusLDescriptor->UniqueID = 3983;
      ZynChorusLDescriptor->Label = "ZynChorus";
      ZynChorusLDescriptor->Properties =  
      LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
      ZynChorusLDescriptor->Name = "ZynChorus";
      ZynChorusLDescriptor->Maker = "Josep Andreu <holborn@telefonica.net>";
      ZynChorusLDescriptor->Copyright = "GNU General Public License version 2";
      ZynChorusLDescriptor->PortCount = 16;
      port_descriptors =
	(LADSPA_PortDescriptor *)
	calloc (ZynChorusLDescriptor->PortCount,
		sizeof (LADSPA_PortDescriptor));
      ZynChorusLDescriptor->PortDescriptors =
	(const LADSPA_PortDescriptor *) port_descriptors;
      port_range_hints =
	(LADSPA_PortRangeHint *) calloc (ZynChorusLDescriptor->PortCount,
					 sizeof (LADSPA_PortRangeHint));
      ZynChorusLDescriptor->PortRangeHints =
	(const LADSPA_PortRangeHint *) port_range_hints;
      port_names =
	(char **) calloc (ZynChorusLDescriptor->PortCount, sizeof (char *));
      ZynChorusLDescriptor->PortNames = (const char **) port_names;

      /* Parameters for Input L */
      port_descriptors[ZynChorus_INPUT_L] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[ZynChorus_INPUT_L] = "Input L";
      port_range_hints[ZynChorus_INPUT_L].HintDescriptor = 0;

      /* Parameters for Input R */
      port_descriptors[ZynChorus_INPUT_R] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[ZynChorus_INPUT_R] = "Input R";
      port_range_hints[ZynChorus_INPUT_R].HintDescriptor = 0;

      /* Parameters for Output L */
      port_descriptors[ZynChorus_OUTPUT_L] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[ZynChorus_OUTPUT_L] = "Output L";
      port_range_hints[ZynChorus_OUTPUT_L].HintDescriptor = 0;

      /* Parameters for Output R */
      port_descriptors[ZynChorus_OUTPUT_R] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[ZynChorus_OUTPUT_R] = "Output R";
      port_range_hints[ZynChorus_OUTPUT_R].HintDescriptor = 0;

      /* Parameters for Dry */
      port_descriptors[ZynChorus_DRY] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynChorus_DRY] = "Dry";
      port_range_hints[ZynChorus_DRY].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE; 
      port_range_hints[ZynChorus_DRY].LowerBound = 0.0;
      port_range_hints[ZynChorus_DRY].UpperBound = 2.0;

      /* Parameters for Wet */
      port_descriptors[ZynChorus_WET] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynChorus_WET] = "Wet";
      port_range_hints[ZynChorus_WET].HintDescriptor =
	LADSPA_HINT_DEFAULT_MAXIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynChorus_WET].LowerBound = 0.0;
      port_range_hints[ZynChorus_WET].UpperBound = 2.0;

      /* Parameters for Pan */
      port_descriptors[ZynChorus_PAN] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynChorus_PAN] = "Pan";
      port_range_hints[ZynChorus_PAN].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynChorus_PAN].LowerBound = 0.0;
      port_range_hints[ZynChorus_PAN].UpperBound = 1.0;

      /* Parameters for Depth */
      port_descriptors[ZynChorus_DEPTH] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynChorus_DEPTH] = "Depth";
      port_range_hints[ZynChorus_DEPTH].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MINIMUM;
      port_range_hints[ZynChorus_DEPTH].LowerBound = 0.0;
      port_range_hints[ZynChorus_DEPTH].UpperBound = 1.0;

      /* Parameters for Delay */
      port_descriptors[ZynChorus_DELAY] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynChorus_DELAY] = "Delay";
      port_range_hints[ZynChorus_DELAY].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MINIMUM;
      port_range_hints[ZynChorus_DELAY].LowerBound = 0.0;
      port_range_hints[ZynChorus_DELAY].UpperBound = 1.0;

      /* Parameters for FB */
      port_descriptors[ZynChorus_FB] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynChorus_FB] = "Feedback";
      port_range_hints[ZynChorus_FB].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynChorus_FB].LowerBound = -0.99;
      port_range_hints[ZynChorus_FB].UpperBound = 0.99;

      /* Parameters for LRCROSS */
      port_descriptors[ZynChorus_LRCROSS] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynChorus_LRCROSS] = "LR Cross";
      port_range_hints[ZynChorus_LRCROSS].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE | LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynChorus_LRCROSS].LowerBound = 0.0;
      port_range_hints[ZynChorus_LRCROSS].UpperBound = 1.0;

      /* Parameters for RANDOM */
      port_descriptors[ZynChorus_RANDOM] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynChorus_RANDOM] = "Randomness";
      port_range_hints[ZynChorus_RANDOM].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynChorus_RANDOM].LowerBound = 0.0;
      port_range_hints[ZynChorus_RANDOM].UpperBound = 1.0;

      /* Parameters for LFOTYPE */
      port_descriptors[ZynChorus_LFOTYPE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynChorus_LFOTYPE] = "Type LFO";
      port_range_hints[ZynChorus_LFOTYPE].HintDescriptor =
	LADSPA_HINT_DEFAULT_0 | LADSPA_HINT_TOGGLED;
      port_range_hints[ZynChorus_LFOTYPE].LowerBound = 0;
      port_range_hints[ZynChorus_LFOTYPE].UpperBound = 1;

      /* Parameters for STEREO */
      port_descriptors[ZynChorus_STEREO] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynChorus_STEREO] = "Stereo";
      port_range_hints[ZynChorus_STEREO].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynChorus_STEREO].LowerBound = -0.5;
      port_range_hints[ZynChorus_STEREO].UpperBound = 0.5;


      /* Parameters for LFOFREQ */
      port_descriptors[ZynChorus_LFOFREQ] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynChorus_LFOFREQ] = "LFO Frequency";
      port_range_hints[ZynChorus_LFOFREQ].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynChorus_LFOFREQ].LowerBound = 0.0;
      port_range_hints[ZynChorus_LFOFREQ].UpperBound = 1.0;

      
      /* Parameters for SUBTRACT */
      port_descriptors[ZynChorus_SUBTRACT] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynChorus_SUBTRACT] = "Subtract";
      port_range_hints[ZynChorus_SUBTRACT].HintDescriptor =
	LADSPA_HINT_DEFAULT_0 | LADSPA_HINT_TOGGLED;
      port_range_hints[ZynChorus_SUBTRACT].LowerBound = 0;
      port_range_hints[ZynChorus_SUBTRACT].UpperBound = 1;

      
      ZynChorusLDescriptor->instantiate = instantiateZynChorus;
      ZynChorusLDescriptor->connect_port = connectPortZynChorus;
      ZynChorusLDescriptor->activate = activateZynChorus;
      ZynChorusLDescriptor->run = runZynChorusWrapper;
      ZynChorusLDescriptor->run_adding = NULL;
      ZynChorusLDescriptor->set_run_adding_gain = NULL;
      ZynChorusLDescriptor->deactivate = ZynChorus_deactivate;
      ZynChorusLDescriptor->cleanup = cleanupZynChorus;
    }



}


void
__attribute__((destructor))
zynchorus_fini()
{
  if (ZynChorusLDescriptor)
    {
      free ((LADSPA_PortDescriptor *) ZynChorusLDescriptor->PortDescriptors);
      free ((char **) ZynChorusLDescriptor->PortNames);
      free ((LADSPA_PortRangeHint *) ZynChorusLDescriptor->PortRangeHints);
      free (ZynChorusLDescriptor);
    }
}



