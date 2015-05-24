/*
  zynreverb.c - ZynAddSubFx Reverb LADSPA plugin software

  zynreverb.c  -  Main LADSPA file of the plugin
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

rdff19 ZynReverb_DRY 4
rdff19 ZynReverb_WET 5
rdff19 ZynReverb_PAN 6
rdff19 ZynReverb_TIME 7
rdff19 ZynReverb_IDEL 8
rdff19 ZynReverb_EARLYREF 9
rdff19 ZynReverb_ROOMSIZE 10
rdff19 ZynReverb_LPF_FREQ 11
rdff19 ZynReverb_HPF_FREQ 12
rdff19 ZynReverb_DAMP 13


*/


#include <stdio.h>
#include <stdlib.h>
#include <ladspa.h>
#include "reverb.h"


#define ZynReverb_INPUT_L 0
#define ZynReverb_INPUT_R 1
#define ZynReverb_OUTPUT_L 2
#define ZynReverb_OUTPUT_R 3
#define ZynReverb_DRY 4
#define ZynReverb_WET 5
#define ZynReverb_PAN 6
#define ZynReverb_TIME 7
#define ZynReverb_IDEL 8
#define ZynReverb_EARLYREF 9
#define ZynReverb_ROOMSIZE 10
#define ZynReverb_LPF_FREQ 11
#define ZynReverb_HPF_FREQ 12
#define ZynReverb_DAMP 13



static LADSPA_Descriptor *ZynReverbLDescriptor = NULL;

static void runZynReverb (LADSPA_Handle instance, unsigned long sample_count);

static void cleanupZynReverb (LADSPA_Handle instance);


const LADSPA_Descriptor *
ladspa_descriptor (unsigned long index)
{
  switch (index)
    {
    case 0:
      return ZynReverbLDescriptor;
    default:
      return NULL;
    }
}


static void
cleanupZynReverb (LADSPA_Handle instance)
{
  ZReverb_t *reverb = (ZReverb_t *) instance;
  free (reverb);
}

static void
connectPortZynReverb (LADSPA_Handle instance, unsigned long port,
		    LADSPA_Data * data)
{
  ZReverb_t *plugin = (ZReverb_t *) instance;
  switch (port)
    {
    case ZynReverb_INPUT_L:
      plugin->inputl = data;
      break;
    case ZynReverb_INPUT_R:
      plugin->inputr = data;
      break;
    case ZynReverb_OUTPUT_L:
      plugin->efxoutl = data;
      break;
    case ZynReverb_OUTPUT_R:
      plugin->efxoutr = data;
      break;
    case ZynReverb_DRY:
      plugin->dry = data;
      break;
    case ZynReverb_WET:
      plugin->outvolume = data;
      break;
    case ZynReverb_PAN:
      plugin->panning = data;
      break;
    case ZynReverb_TIME:
      plugin->Ptime = data;
      break;    
    case ZynReverb_IDEL:
      plugin->Pidelay = data;
      break;
    case ZynReverb_EARLYREF:
      plugin->idelayfb = data;
      break;
    case ZynReverb_ROOMSIZE:
      plugin->Proomsize = data;
      break;
    case ZynReverb_LPF_FREQ:
      plugin->Plpf = data;
      break;
    case ZynReverb_HPF_FREQ:
      plugin->Phpf = data;
      break;
    case ZynReverb_DAMP:
      plugin->Plohidamp = data;
      break;

    }
}



static LADSPA_Handle
instantiateZynReverb (const LADSPA_Descriptor * descriptor,
		    unsigned long s_rate)
{



  ZReverb_t *reverb = (ZReverb_t *) calloc (1, sizeof (ZReverb_t));
  
  if (reverb)
  {
  reverb->SAMPLE_RATE = s_rate;
  Reverb_Init (reverb);

  }

  return (LADSPA_Handle) reverb;
}

static void
activateZynReverb (LADSPA_Handle instance)
{

//  ZReverb_t *reverb = (ZReverb_t *) instance;


}


void
ZynReverb_deactivate (LADSPA_Handle instance)
{

//  ZReverb_t *reverb = (ZReverb_t *) instance;


}


static void
runZynReverbWrapper (LADSPA_Handle instance, unsigned long sample_count)
{
  runZynReverb (instance, sample_count);

}

static void
runZynReverb (LADSPA_Handle instance, unsigned long sample_count)
{

  ZReverb_t *reverb = (ZReverb_t *) instance;

  LADSPA_Data *const pinputl = reverb->inputl;
  LADSPA_Data *const pinputr = reverb->inputr;
  
  int i;
  
   for (i=0; i < sample_count; i++)
      {
        reverb->indatal[i] = pinputl[i];
        reverb->indatar[i] = pinputr[i];
      }
 
   out(reverb, pinputl,pinputr,sample_count);

}


void __attribute__ ((constructor))
zynreverb_init ()

{


  char **port_names;
  LADSPA_PortDescriptor *port_descriptors;
  LADSPA_PortRangeHint *port_range_hints;
  ZynReverbLDescriptor =
    (LADSPA_Descriptor *) malloc (sizeof (LADSPA_Descriptor));
  if (ZynReverbLDescriptor)
    {
      ZynReverbLDescriptor->UniqueID = 3986;
      ZynReverbLDescriptor->Label = "ZynReverb";
      ZynReverbLDescriptor->Properties =  
      LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
      ZynReverbLDescriptor->Name = "ZynReverb";
      ZynReverbLDescriptor->Maker = "Josep Andreu <holborn@telefonica.net>";
      ZynReverbLDescriptor->Copyright = "GNU General Public License version 2";
      ZynReverbLDescriptor->PortCount = 14;
      port_descriptors =
	(LADSPA_PortDescriptor *)
	calloc (ZynReverbLDescriptor->PortCount,
		sizeof (LADSPA_PortDescriptor));
      ZynReverbLDescriptor->PortDescriptors =
	(const LADSPA_PortDescriptor *) port_descriptors;
      port_range_hints =
	(LADSPA_PortRangeHint *) calloc (ZynReverbLDescriptor->PortCount,
					 sizeof (LADSPA_PortRangeHint));
      ZynReverbLDescriptor->PortRangeHints =
	(const LADSPA_PortRangeHint *) port_range_hints;
      port_names =
	(char **) calloc (ZynReverbLDescriptor->PortCount, sizeof (char *));
      ZynReverbLDescriptor->PortNames = (const char **) port_names;

      /* Parameters for Input L */
      port_descriptors[ZynReverb_INPUT_L] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[ZynReverb_INPUT_L] = "Input L";
      port_range_hints[ZynReverb_INPUT_L].HintDescriptor = 0;

      /* Parameters for Input R */
      port_descriptors[ZynReverb_INPUT_R] =
	LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
      port_names[ZynReverb_INPUT_R] = "Input R";
      port_range_hints[ZynReverb_INPUT_R].HintDescriptor = 0;

      /* Parameters for Output L */
      port_descriptors[ZynReverb_OUTPUT_L] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[ZynReverb_OUTPUT_L] = "Output L";
      port_range_hints[ZynReverb_OUTPUT_L].HintDescriptor = 0;

      /* Parameters for Output R */
      port_descriptors[ZynReverb_OUTPUT_R] =
	LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
      port_names[ZynReverb_OUTPUT_R] = "Output R";
      port_range_hints[ZynReverb_OUTPUT_R].HintDescriptor = 0;

      /* Parameters for Dry */
      port_descriptors[ZynReverb_DRY] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynReverb_DRY] = "Dry";
      port_range_hints[ZynReverb_DRY].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE; 
      port_range_hints[ZynReverb_DRY].LowerBound = 0.0;
      port_range_hints[ZynReverb_DRY].UpperBound = 2.0;

      /* Parameters for Wet */
      port_descriptors[ZynReverb_WET] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynReverb_WET] = "Wet";
      port_range_hints[ZynReverb_WET].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynReverb_WET].LowerBound = 0.0;
      port_range_hints[ZynReverb_WET].UpperBound = 2.0;

      /* Parameters for Pan */
      port_descriptors[ZynReverb_PAN] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynReverb_PAN] = "Pan";
      port_range_hints[ZynReverb_PAN].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MIDDLE;
      port_range_hints[ZynReverb_PAN].LowerBound = 0.0;
      port_range_hints[ZynReverb_PAN].UpperBound = 1.0;

      /* Parameters for Time */
      port_descriptors[ZynReverb_TIME] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynReverb_TIME] = "Reverb Time";
      port_range_hints[ZynReverb_TIME].HintDescriptor =
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE |
	LADSPA_HINT_DEFAULT_MINIMUM;
      port_range_hints[ZynReverb_TIME].LowerBound = 0.01;
      port_range_hints[ZynReverb_TIME].UpperBound = 20.0;

      /* Parameters for Inicial Delay */
      port_descriptors[ZynReverb_IDEL] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynReverb_IDEL] = "Initial Delay";
      port_range_hints[ZynReverb_IDEL].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynReverb_IDEL].LowerBound = 0.0;
      port_range_hints[ZynReverb_IDEL].UpperBound = 1.0;

      /* Parameters for Early Reference */
      port_descriptors[ZynReverb_EARLYREF] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynReverb_EARLYREF] = "Delay E/R";
      port_range_hints[ZynReverb_EARLYREF].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM | LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynReverb_EARLYREF].LowerBound = 0.0;
      port_range_hints[ZynReverb_EARLYREF].UpperBound = 0.99;

      /* Parameters for Room Size */
      port_descriptors[ZynReverb_ROOMSIZE] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynReverb_ROOMSIZE] = "Room Size";
      port_range_hints[ZynReverb_ROOMSIZE].HintDescriptor =
	LADSPA_HINT_DEFAULT_MAXIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynReverb_ROOMSIZE].LowerBound = -1.0;
      port_range_hints[ZynReverb_ROOMSIZE].UpperBound = 0.0;

      /* Parameters for LPF Frequeny */
      port_descriptors[ZynReverb_LPF_FREQ] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynReverb_LPF_FREQ] = "LPF Frequency";
      port_range_hints[ZynReverb_LPF_FREQ].HintDescriptor =
	LADSPA_HINT_DEFAULT_MAXIMUM | 
	 LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynReverb_LPF_FREQ].LowerBound = 21.0;
      port_range_hints[ZynReverb_LPF_FREQ].UpperBound = 10020.0;

      /* Parameters for HPF Frequency */
      port_descriptors[ZynReverb_HPF_FREQ] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynReverb_HPF_FREQ] = "HPF Frequency";
      port_range_hints[ZynReverb_HPF_FREQ].HintDescriptor =
	LADSPA_HINT_DEFAULT_MINIMUM |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynReverb_HPF_FREQ].LowerBound = 21.0;
      port_range_hints[ZynReverb_HPF_FREQ].UpperBound = 10020.0;

      /* Parameters for DAMP */
      port_descriptors[ZynReverb_DAMP] =
	LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
      port_names[ZynReverb_DAMP] = "Damp";
      port_range_hints[ZynReverb_DAMP].HintDescriptor =
	LADSPA_HINT_DEFAULT_MIDDLE |
	LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE;
      port_range_hints[ZynReverb_DAMP].LowerBound = -1.0;
      port_range_hints[ZynReverb_DAMP].UpperBound = 1.0;

      ZynReverbLDescriptor->instantiate = instantiateZynReverb;
      ZynReverbLDescriptor->connect_port = connectPortZynReverb;
      ZynReverbLDescriptor->activate = activateZynReverb;
      ZynReverbLDescriptor->run = runZynReverbWrapper;
      ZynReverbLDescriptor->run_adding = NULL;
      ZynReverbLDescriptor->set_run_adding_gain = NULL;
      ZynReverbLDescriptor->deactivate = ZynReverb_deactivate;
      ZynReverbLDescriptor->cleanup = cleanupZynReverb;
    }



}

void __attribute__ ((destructor))
zynreverb_fini ()


{
  if (ZynReverbLDescriptor)
    {
      free ((LADSPA_PortDescriptor *) ZynReverbLDescriptor->PortDescriptors);
      free ((char **) ZynReverbLDescriptor->PortNames);
      free ((LADSPA_PortRangeHint *) ZynReverbLDescriptor->PortRangeHints);
      free (ZynReverbLDescriptor);
    }
}



