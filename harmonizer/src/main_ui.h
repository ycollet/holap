
/*
  holharm - a intelligent harmonizer plugin software

  main_ui.h  -  Variable Definitions and functions
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
 (version2)  along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/


#ifndef _MAIN_UI_H
#define _MAIN_UI_H

#include <lo/lo.h>
#include "../config.h"
#include "holharm_ui.h"

#define QUALITY 4

void read_stdin (void);
void update_widgets (int port, float value);
void initchords ();
void update_sel_chord (int note, int type);
void update_mode (int type);
void ReadConfig();

typedef struct
{
  int tipo;
  int num;
  char Nom[16];
} TipoCh;

typedef struct
{
  char Nom[3];
} NomChord;



holharm_ui gui;
NomChord NC[12];
TipoCh ChN[34];
lo_server osc_server;
lo_address m_host;
char *osc_configure_path;
char *osc_control_path;
char *osc_exiting_path;
char *osc_hide_path;
char *osc_midi_path;
char *osc_program_path;
char *osc_quit_path;
char *osc_show_path;
char *osc_update_path;
char *myurl;
int funcion;
int Quality;
pthread_mutex_t mutex;


#endif
