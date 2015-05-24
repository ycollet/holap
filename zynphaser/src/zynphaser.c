/*
  zynphaser.c - ZynAddSubFx Phaser LADSPA plugin software

  zynphaser.c  -  Main LADSPA file of the plugin
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

rdff19 ZynPhaser_DRY 4
rdff19 ZynPhaser_WET 5
rdff19 ZynPhaser_PAN 6
rdff19 ZynPhaser_DEPTH 7
rdff19 ZynPhaser_FB 8
rdff19 ZynPhaser_LRCROSS 9
rdff19 ZynPhaser_RANDOM 10
rdff19 ZynPhaser_LFOTYPE 11
rdff19 ZynPhaser_STEREO 12
rdff19 ZynPhaser_LFOFREQ 13
rdff19 ZynPhaser_PHASE 14
rdff19 ZynPhaser_SUBTRACT 15
rdff19 ZynPhaser_STAGES 16


*/


#include <stdio.h>
#include <stdlib.h>
#include <ladspa.h>
#include "phaser.h"


#define ZynPhaser_INPUT_L 0
#define ZynPhaser_INPUT_R 1
#define ZynPhaser_OUTPUT_L 2
#define ZynPhaser_OUTPUT_R 3
#define ZynPhaser_DRY 4
#define ZynPhaser_WET 5
#define ZynPhaser_PAN 6
#define ZynPhaser_DEPTH 7
#define ZynPhaser_FB 8
#define ZynPhaser_LRCROSS 9
#define ZynPhaser_RANDOM 10
#define ZynPhaser_LFOTYPE 11
#define ZynPhaser_STEREO 12
#define ZynPhaser_LFOFREQ 13
#define ZynPhaser_PHASE 14
#define ZynPhaser_SUBTRACT 15
#define ZynPhaser_STAGES 16







static LADSPA_Descriptor *ZynPhaserLDescriptor = NULL;

static void runZynPhaser (LADSPA_Handle instance, unsigned long sample_count);

static void cleanupZynPhaser (LADSPA_Handle instance);


const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return ZynPhaserLDescriptor;
    default:
      return NULL;
    }
}


static void
cleanupZynPhaser (LADSPA_Handle instance)
{
  ZPhaser_t *phaser = (ZPhaser_t *) instance;
  free (phaser);
}

static void
connectPortZynPhaser (LADSPA_Handle instance, unsigned long port,
		    LADSPA_Data * data)
{
  ZPhaser_t *plugin = (ZPhaser_t *) instance;
  switch (port)
    {
    case ZynPhaser_INPUT_L:
      plugin->inputl = data;
      break;
    case ZynPhaser_INPUT_R:
      plugin->inputr = data;
      break;
    case ZynPhaser_OUTPUT_L:
      plugin->efxoutl = data;
      break;
    case ZynPhaser_OUTPUT_R:
      plugin->efxoutr = data;
      break;
    case ZynPhaser_DRY:
      plugin->dry = data;
      break;
    case ZynPhaser_WET:
      plugin->outvolume = data;
      break;
    case ZynPhaser_PAN:
      plugin->panning = data;
      break;
    case ZynPhaser_DEPTH:
      plugin->depth = data;
      break;
    case ZynPhaser_FB:
      plugin->fb = data;
      break;
    case ZynPhaser_LRCROSS:
      plugin->lrcross = data;
      break;
    case ZynPhaser_RANDOM:
      plugin->lfornd = data;
      break;
    case ZynPhaser_LFOTYPE:
      plugin->PLFOtype = data;
      break;
    case ZynPhaser_STEREO:
      plugin->Pstereo = data;
      break;
    case ZynPhaser_LFOFREQ:
      plugin->lfofreq = data;
      break;
    case ZynPhaser_PHASE:
      plugin->Phase = data;
      break;    
    case ZynPhaser_SUBTRACT:
      plugin->Poutsub = data;
      break;
    case ZynPhaser_STAGES:
      plugin->Pstages = data;
      break;

    }
}



static LADSPA_Handle
instantiateZynPhaser (const LADSPA_Descriptor * descriptor,
		    unsigned long s_rate)
{



  ZPhaser_t *phaser = (ZPhaser_t *) calloc (1, sizeof (ZPhaser_t));
  
  if (phaser)
  {
  phaser->SAMPLE_RATE = s_rate;
  Phaser_Init (phaser);
  EffectLFO_Init(phaser); 
  }

  return (LADSPA_Handle) phaser;
}

static void
activateZynPhaser (LADSPA_Handle instance)
{

//  ZPhaser_t *phaser = (ZPhaser_t *) instance;


}


void
ZynPhaser_deactivate (LADSPA_Handle instance)
{

//  ZPhaser_t *phaser = (ZPhaser_t *) instance;


}


static void
runZynPhaserWrapper (LADSPA_Handle instance, unsigned long sample_count)
{
  runZynPhaser (instance, sample_count);

}

static void
runZynPhaser (LADSPA_Handle instance, unsigned long sample_count)
{

  ZPhaser_t *phaser = (ZPhaser_t *) instance;

  LADSPA_Data *const outputl = phaser->efxoutl;
  LADSPA_Data *const outputr = phaser->efxoutr;
  LADSPA_Data *const pinputl = phaser->inputl;
  LADSPA_Data *const pinputr = phaser->inputr;
  LADSPA_Data dry = *(phaser->dry);
  LADSPA_Data wet = *(phaser->outvolume);

  int i;

       for (i = 0; i < sample_count; i++)
               {
                 phaser->indatal[i]=pinputl[i];
                 phaser->indatar[i]=pinputr[i];
               }

    out(phaser, pinputl,pinputr,sample_count);
        
        
        for (i = 0; i < sample_count; i++)
         {       
          outputl[i] = phaser->indatal[i]*dry + outputl[i]*wet;
          outputr[i] = phaser->indatar[i]*dry + outputr[i]*wet;
        }
  


}


void __attribute__ ((constructor))
zynphaser_init ()

{


  char **port_names;
  LADSPA_PortDescriptor *port_descriptors;
  LADSPA_PortRangeHint *port_range_hints;
  ZynPhaserLDescriptor =
    (LADSPA_Descriptor *) malloc (sizeof (LADSPA_Descriptor));
  if (ZynPhaserLDescriptor)
    {
      ZynPhaserLDescriptor->UniqueID = 3985;
      ZynPhaserLDescriptor->Label = "ZynPhaser";
      ZynPhaserLDescriptor->Properties =  
      LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
      ZynPhaserLDescriptor->Name = "ZynPhaser";
      ZynPhaserLDescriptor->Maker = "Josep Andreu <holborn@telefonica.net>";
      ZynPhaserLDescriptor->Copyright = "GNU General Public License version 2";
      ZynPhaserLDescriptor->PortCount = 17;
      port_descriptors =
	(LADSPA_PortDescriptor *)
	calloc (ZynPhaserLDescriptor->PortCount,
		sizeof (LADSPA_PortDescriptor));
      ZynPhaserLDescriptor->PortDescriptors =
	(const LADSPA_PortDescriptor *) port_descriptors;
      port_range_hints =
	(LADSPA_PortRangeHint *) calloc (ZynPhaserLDescriptor->PortCount,
					 sizeof (LADSPA_PortRangeHint));
      ZynPhaserLDescriptor->PortRangeHints =
	(const LADSPA_PortRangeHint *) port_range_hints;
      port_names =
	(char **) calloc (ZynPhaserLDescriptor->PortCount, sizeof (char *));
      ZynPhaserLDescriptor->PortNames = (const char **) port_names;

      /* Parameters for Input L */
      port_descriptors[ZynPhaser_INPUT_L] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[ZynPhaser_INPUT_L] = "Input L";
      port_range_hints[ZynPhaser_INPUT_L].HintDescriptor = 0;

      /* Parameters for Input R */
      port_descriptors[ZynPhaser_INPUT_R] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[ZynPhaser_INPUT_R] = "Input R";
      port_range_hints[ZynPhaser_INPUT_R].HintDescriptor = 0;

      /* Parameters for Output L */
      port_descriptors[ZynPhaser_OUTPUT_L] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[ZynPhaser_OUTPUT_L] = "Output L";
      port_range_hints[ZynPhaser_OUTPUT_L].HintDescriptor = 0;

      /* Parameters for Output R */
      port_descriptors[ZynPhaser_OUTPUT_R] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[ZynPhaser_OUTPUT_R] = "Output R";
      port_range_hints[ZynPhaser_OUTPUT_R].HintDescriptor = 0;

      /* Parameters for Dry */
      port_descriptors[ZynPhaser_DRY] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_DRY] = "Dry";
      port_range_hints[ZynPhaser_DRY].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE; 
      port_range_hints[ZynPhaser_DRY].LowerBound = 0.0;
      port_range_hints[ZynPhaser_DRY].UpperBound = 2.0;

      /* Parameters for Volume */
      port_descriptors[ZynPhaser_WET] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_WET] = "Wet";
      port_range_hints[ZynPhaser_WET].HintDescriptor =
	LADSPA_HINT_DEFAULT_MAXIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynPhaser_WET].LowerBound = 0.0;
      port_range_hints[ZynPhaser_WET].UpperBound = 2.0;

      /* Parameters for Pan */
      port_descriptors[ZynPhaser_PAN] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_PAN] = "Pan";
      port_range_hints[ZynPhaser_PAN].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynPhaser_PAN].LowerBound = 0.0;
      port_range_hints[ZynPhaser_PAN].UpperBound = 1.0;

      /* Parameters for Depth */
      port_descriptors[ZynPhaser_DEPTH] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_DEPTH] = "Depth";
      port_range_hints[ZynPhaser_DEPTH].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MINIMUM;
      port_range_hints[ZynPhaser_DEPTH].LowerBound = 0.0;
      port_range_hints[ZynPhaser_DEPTH].UpperBound = 1.0;

      /* Parameters for FB */
      port_descriptors[ZynPhaser_FB] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_FB] = "Feedback";
      port_range_hints[ZynPhaser_FB].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynPhaser_FB].LowerBound = -0.99;
      port_range_hints[ZynPhaser_FB].UpperBound = 0.99;

      /* Parameters for LRCROSS */
      port_descriptors[ZynPhaser_LRCROSS] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_LRCROSS] = "LR Cross";
      port_range_hints[ZynPhaser_LRCROSS].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynPhaser_LRCROSS].LowerBound = 0.0;
      port_range_hints[ZynPhaser_LRCROSS].UpperBound = 1.0;

      /* Parameters for RANDOM */
      port_descriptors[ZynPhaser_RANDOM] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_RANDOM] = "Randomness";
      port_range_hints[ZynPhaser_RANDOM].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynPhaser_RANDOM].LowerBound = 0.0;
      port_range_hints[ZynPhaser_RANDOM].UpperBound = 1.0;

      /* Parameters for LFOTYPE */
      port_descriptors[ZynPhaser_LFOTYPE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_LFOTYPE] = "Type LFO";
      port_range_hints[ZynPhaser_LFOTYPE].HintDescriptor =
	LADSPA_HINT_DEFAULT_0 | LADSPA_HINT_TOGGLED;
      port_range_hints[ZynPhaser_LFOTYPE].LowerBound = 0;
      port_range_hints[ZynPhaser_LFOTYPE].UpperBound = 1;

      /* Parameters for STEREO */
      port_descriptors[ZynPhaser_STEREO] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_STEREO] = "Stereo df";
      port_range_hints[ZynPhaser_STEREO].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynPhaser_STEREO].LowerBound = -0.5;
      port_range_hints[ZynPhaser_STEREO].UpperBound = 0.5;


      /* Parameters for LFOFREQ */
      port_descriptors[ZynPhaser_LFOFREQ] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_LFOFREQ] = "LFO Frequency";
      port_range_hints[ZynPhaser_LFOFREQ].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynPhaser_LFOFREQ].LowerBound = 0.0;
      port_range_hints[ZynPhaser_LFOFREQ].UpperBound = 1.0;

      
      /* Parameters for PHASE */
      port_descriptors[ZynPhaser_PHASE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_PHASE] = "Phase";
      port_range_hints[ZynPhaser_PHASE].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE | 
       	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynPhaser_PHASE].LowerBound = 0.0;
      port_range_hints[ZynPhaser_PHASE].UpperBound = 1.0;

      /* Parameters for SUBTRACT */
      port_descriptors[ZynPhaser_SUBTRACT] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_SUBTRACT] = "Subtract";
      port_range_hints[ZynPhaser_SUBTRACT].HintDescriptor =
	LADSPA_HINT_DEFAULT_0 | LADSPA_HINT_TOGGLED;
      port_range_hints[ZynPhaser_SUBTRACT].LowerBound = 0;
      port_range_hints[ZynPhaser_SUBTRACT].UpperBound = 1;

      /* Parameters for Stages */
      port_descriptors[ZynPhaser_STAGES] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynPhaser_STAGES] = "Stages";
      port_range_hints[ZynPhaser_STAGES].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_INTEGER;
      port_range_hints[ZynPhaser_STAGES].LowerBound = 1.1;
      port_range_hints[ZynPhaser_STAGES].UpperBound = 12.1;


      
      ZynPhaserLDescriptor->instantiate = instantiateZynPhaser;
      ZynPhaserLDescriptor->connect_port = connectPortZynPhaser;
      ZynPhaserLDescriptor->activate = activateZynPhaser;
      ZynPhaserLDescriptor->run = runZynPhaserWrapper;
      ZynPhaserLDescriptor->run_adding = NULL;
      ZynPhaserLDescriptor->set_run_adding_gain = NULL;
      ZynPhaserLDescriptor->deactivate = ZynPhaser_deactivate;
      ZynPhaserLDescriptor->cleanup = cleanupZynPhaser;
    }



}

void __attribute__ ((destructor))
zynphaser_fini ()

{
  if (ZynPhaserLDescriptor)
    {
      free ((LADSPA_PortDescriptor *) ZynPhaserLDescriptor->PortDescriptors);
      free ((char **) ZynPhaserLDescriptor->PortNames);
      free ((LADSPA_PortRangeHint *) ZynPhaserLDescriptor->PortRangeHints);
      free (ZynPhaserLDescriptor);
    }
}



