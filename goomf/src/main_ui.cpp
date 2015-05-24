/*
  goomf - a FM mono syhth plugin software

  main_ui.cpp  -  Main dssi GUI file of the plugin
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
(version2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include <unistd.h>
#include "goomf_ui.h"
#include "main_ui.h"




void *
thread1 (void *arg)
{
  int i;
  while (gui.Pexitprogram == 0)
    {
      switch (gui.ready)
	{
	case 1:
	  i = loadbank (gui.uBankFilename);
	  if (!i)
	    Put_Loaded_Bank ();
	  break;
	case 2:
	  gui.Pname->value (Banco[(int) gui.PresetSelect->value ()].Name);
	  Put_Combi (gui.PresetSelect->value ());
	  Send_Values ();
	  Update_Main_Widgets ();
	  Update_Operator_Widgets (gui.op);
	  break;
	case 3:
	  savebank (gui.uBankFilename);
	  break;
	case 4:
	  New_Bank ();
	  Put_Loaded_Bank ();
	  break;
	case 5:
	  make_window_banks ();
	  break;
	case 6:
	  New ();
	  Send_Values ();
	  Update_Main_Widgets ();
	  Update_Operator_Widgets (gui.op);
	  gui.Pname->value ("");
	  break;
	}

      gui.ready = 0;

      switch (funcion)
	{
	case 1:
	  gui.ui_win->iconize ();
	  break;
	case 2:
	  gui.ui_win->show ();
	  break;
	}

      funcion = 0;

      if (gui.cop)
	{
	  Update_Operator_Widgets (gui.op);
	  gui.cop = 0;
	}
      if (gui.D_Vol_c)
	{
	  lo_send (m_host, osc_control_path, "if", 2, gui.D_Vol->value ());
	  master_volume = gui.D_Vol->value ();
	  gui.D_Vol_c = 0;
	}

      if (gui.D_Tune_c)
	{
	  lo_send (m_host, osc_control_path, "if", 3, gui.D_Tune->value ());
	  tune = gui.D_Tune->value ();
	  gui.D_Tune_c = 0;
	}

      if (gui.D_Portamento_c)
	{
	  lo_send (m_host, osc_control_path, "if", 4,
		   gui.D_Portamento->value ());
	  portamento = gui.D_Portamento->value ();
	  gui.D_Portamento_c = 0;
	}
      if (gui.D_LFO_Volume_c)
	{
	  lo_send (m_host, osc_control_path, "if", 5,
		   gui.D_LFO_Volume->value ());
	  LFO_Volume = gui.D_LFO_Volume->value ();
	  gui.D_LFO_Volume_c = 0;
	}
      if (gui.D_LFO_Frequency_c)
	{
	  lo_send (m_host, osc_control_path, "if", 6,
		   gui.D_LFO_Frequency->value ());
	  LFO_Frequency = gui.D_LFO_Frequency->value ();
	  gui.D_LFO_Frequency_c = 0;
	}
      if (gui.D_LFO_Delay_c)
	{
	  lo_send (m_host, osc_control_path, "if", 7,
		   gui.D_LFO_Delay->value ());
	  LFO_Delay = gui.D_LFO_Delay->value ();
	  gui.D_LFO_Delay_c = 0;
	}
      if (gui.D_LFO_Wave_c)
	{
	  lo_send (m_host, osc_control_path, "if", 8,
		   gui.D_LFO_Wave->value ());
	  LFO_Wave = (int) gui.D_LFO_Wave->value ();
	  gui.D_LFO_Wave_c = 0;
	}
      if (gui.D_Wave_c[gui.op])
	{
	  lo_send (m_host, osc_control_path, "if", 9 + gui.op,
		   gui.D_Wave->value ());
	  wave[gui.op] = (int) gui.D_Wave->value ();
	  gui.D_Wave_c[gui.op] = 0;
	}
      if (gui.D_H_c[gui.op])
	{
	  lo_send (m_host, osc_control_path, "if", 15 + gui.op,
		   gui.D_H->value ());
	  H[gui.op] = gui.D_H->value ();
	  gui.D_H_c[gui.op] = 0;
	}

      if (gui.D_HF_c[gui.op])
	{
	  lo_send (m_host, osc_control_path, "if", 21 + gui.op,
		   gui.D_HF->value ());
	  HF[gui.op] = gui.D_HF->value ();
	  gui.D_HF_c[gui.op] = 0;
	}

      if (gui.D_OVol_c[gui.op])
	{
	  lo_send (m_host, osc_control_path, "if", 27 + gui.op,
		   gui.D_OVol->value ());
	  Ovol[gui.op] = gui.D_OVol->value ();
	  gui.D_OVol_c[gui.op] = 0;
	}

      if (gui.D_Attack_c[gui.op])
	{
	  lo_send (m_host, osc_control_path, "if", 33 + gui.op,
		   gui.D_Attack->value ());
	  attack[gui.op] = gui.D_Attack->value ();
	  gui.D_Attack_c[gui.op] = 0;
	}

      if (gui.D_Decay_c[gui.op])
	{
	  lo_send (m_host, osc_control_path, "if", 39 + gui.op,
		   gui.D_Decay->value ());
	  decay[gui.op] = gui.D_Decay->value ();
	  gui.D_Decay_c[gui.op] = 0;
	}

      if (gui.D_Sustain_c[gui.op])
	{
	  lo_send (m_host, osc_control_path, "if", 45 + gui.op,
		   gui.D_Sustain->value ());
	  sustain[gui.op] = gui.D_Sustain->value ();
	  gui.D_Sustain_c[gui.op] = 0;
	}

      if (gui.D_Release_c[gui.op])
	{
	  lo_send (m_host, osc_control_path, "if", 51 + gui.op,
		   gui.D_Release->value ());
	  release[gui.op] = gui.D_Release->value ();
	  gui.D_Release_c[gui.op] = 0;
	}

      if (gui.D_pLFO_c[gui.op])
	{
	  lo_send (m_host, osc_control_path, "if", 57 + gui.op,
		   gui.D_pLFO->value ());
	  pLFO[gui.op] = gui.D_pLFO->value ();
	  gui.D_pLFO_c[gui.op] = 0;
	}

      if (gui.D_Ftype_c)
	{
	  lo_send (m_host, osc_control_path, "if", 63, gui.D_Ftype->value ());
	  Ftype = gui.D_Ftype->value ();
	  set_FilterType (Ftype);
	  gui.D_Ftype_c = 0;
	}

      if (gui.D_Fgain_c)
	{
	  lo_send (m_host, osc_control_path, "if", 64, gui.D_Fgain->value ());
	  Fgain = gui.D_Fgain->value ();
	  gui.D_Fgain_c = 0;
	}

      if (gui.D_Fcutoff_c)
	{
	  lo_send (m_host, osc_control_path, "if", 65,
		   gui.D_Fcutoff->value ());
	  Fcutoff = gui.D_Fcutoff->value ();
	  gui.D_Fcutoff_c = 0;
	}

      if (gui.D_Fq_c)
	{
	  lo_send (m_host, osc_control_path, "if", 66, gui.D_Fq->value ());
	  Fq = gui.D_Fq->value ();
	  gui.D_Fq_c = 0;
	}

      if (gui.D_FLFO_c)
	{
	  lo_send (m_host, osc_control_path, "if", 67, gui.D_FLFO->value ());
	  FLFO = gui.D_FLFO->value ();
	  gui.D_FLFO_c = 0;
	}

      if (gui.D_FADSR_c)
	{
	  lo_send (m_host, osc_control_path, "if", 68, gui.D_FADSR->value ());
	  FADSR = gui.D_FADSR->value ();
	  gui.D_FADSR_c = 0;
	}

      if (gui.D_Fstages_c)
	{
	  lo_send (m_host, osc_control_path, "if", 69,
		   gui.D_Fstages->value ());
	  Fstages = gui.D_Fstages->value ();
	  gui.D_Fstages_c = 0;
	}

      if (gui.D_Fvelocity_c)
	{
	  lo_send (m_host, osc_control_path, "if", 70,
		   (float) gui.D_Fvelocity->value ());
	  Fvelocity = gui.D_Fvelocity->value ();
	  gui.D_Fvelocity_c = 0;
	}

      if (gui.D_Algo_c)
        {
         lo_send (m_host, osc_control_path, "if", 71,
		   (float) gui.D_Algo->value ());
	  algo = gui.D_Algo->value ();
	  gui.D_Algo_c = 0;
        }

      if(gui.Pexit)
        {
         gui.Pexit=0;
         lo_send (m_host, osc_exiting_path, "");
         exit(0);
        } 

      Fl::wait ();
    }

  

};



void
update_widgets (int port, float value)
{

  switch (port)
    {
    case 2:
      gui.D_Vol->value (value);
      break;
    case 3:
      gui.D_Tune->value (value);
      break;
    case 4:
      gui.D_Portamento->value (value);
      break;
    case 5:
      gui.D_LFO_Volume->value (value);
      break;
    case 6:
      gui.D_LFO_Frequency->value (value);
      break;
    case 7:
      gui.D_LFO_Delay->value (value);
      break;
    case 8:
      gui.D_LFO_Wave->value (value);
      break;
    case 9:
      gui.D_Wave->value (value);
      break;
    case 10:
      gui.D_H->value (value);
      break;
    case 11:
      gui.D_HF->value (value);
      break;
    case 12:
      gui.D_OVol->value (value);
      break;
    case 13:
      gui.D_Attack->value (value);
      break;
    case 14:
      gui.D_Decay->value (value);
      break;
    case 15:
      gui.D_Sustain->value (value);
      break;
    case 16:
      gui.D_Release->value (value);
      break;
    case 17:
      gui.D_pLFO->value (value);
      break;
    case 63:
      gui.D_Ftype->value (value);
      set_FilterType (value);
      break;
    case 64:
      gui.D_Fgain->value (value);
      break;
    case 65:
      gui.D_Fcutoff->value (value);
      break;
    case 66:
      gui.D_Fq->value (value);
      break;
    case 67:
      gui.D_FLFO->value (value);
      break;
    case 68:
      gui.D_FADSR->value (value);
      break;
    case 69:
      gui.D_Fstages->value (value);
      break;
    case 70:
      gui.D_Fvelocity->value (value);
      break;
    case 71:
      gui.D_Algo->value(value);
      break; 
 
    }


}

static char *
osc_build_path (char *base_path, const char *method)
{
  char buffer[256];
  char *full_path;

  snprintf (buffer, 256, "%s%s", base_path, method);
  if (!(full_path = strdup (buffer)))
    {

      exit (1);
    }
  return strdup (buffer);
}


static void
osc_error (int num, const char *msg, const char *path)
{

  printf (" error: liblo server error %d in path \"%s\": %s\n",
	  num, (path ? path : "(null)"), msg);


}




int
debug_handler (const char *path, const char *types, lo_arg ** argv,
	       int argc, void *data, void *user_data)
{
  int i;

  printf ("Warning: unhandled OSC message in GUI:\n");
  for (i = 0; i < argc; ++i)
    {
      printf ("arg  %d : type : %c \n", i, types[i]);
      lo_arg_pp ((lo_type) types[i], argv[i]);

    }

  printf ("path is %s\n", path);
  return 1;
}

int
configure_handler (const char *path, const char *types, lo_arg ** argv,
		   int argc, void *data, void *user_data)
{

  const char *key = (const char *) &argv[0]->s;
  const char *value = (const char *) &argv[1]->s;






}

int
show_handler (const char *path, const char *types, lo_arg ** argv,
	      int argc, void *data, void *user_data)
{
  funcion = 2;
  return 0;
}

int
hide_handler (const char *path, const char *types, lo_arg ** argv,
	      int argc, void *data, void *user_data)
{

  funcion = 1;
  return 0;
}

int
quit_handler (const char *path, const char *types, lo_arg ** argv,
	      int argc, void *data, void *user_data)
{
  gui.Pexitprogram = 1;
  lo_send (m_host, osc_quit_path, "");
  exit(0);
  return 0;

}

int
exiting_handler (const char *path, const char *types, lo_arg ** argv,
		 int argc, void *data, void *user_data)
{
  gui.Pexitprogram = 1;
  lo_send (m_host, osc_exiting_path, "");
  exit(0);
  return 0;
}

int
control_handler (const char *path, const char *types, lo_arg ** argv,
		 int argc, void *data, void *user_data)
{


  if (argc < 2)
    {
      printf ("Error: too few arguments to control_handler\n");
      return 1;
    }

  const int port = argv[0]->i;
  const float value = argv[1]->f;


  if ((port < 9) || (port > 62))
    update_widgets (port, value);
  else if (port < 63)
    update_widgets (((port + 3) / 6) + 7, value);

  return 0;
}

int
program_handler (const char *path, const char *types, lo_arg ** argv,
		 int argc, lo_message msg, void *user_data)
{
  int bank, program, i;

  if (argc < 2)
    {
      printf (" error: too few arguments to osc_program_handler\n");
      return 1;
    }

  bank = argv[0]->i;
  program = argv[1]->i;

  gui.PresetSelect->value (program);
  gui.ready = 2;

  printf
    ("osc_program_handler: received program change, bank %d, program %d\n",
     bank, program);

  return 0;
}


int
main (int argc, char **argv)
{

  int i;
  int lo_fd;
  fd_set rfds;
  struct timeval tv;
  int retval;
  int done = 0;
  int loadOK = 0;

  funcion = 0;
  gui.Pexit = 0 ;
  gui.Pexitprogram = 0;
  gui.ready = 0;
  gui.D_Vol_c = 0;
  gui.D_Tune_c = 0;
  gui.D_Portamento_c = 0;
  gui.D_LFO_Volume_c = 0;
  gui.D_LFO_Frequency_c = 0;
  gui.D_LFO_Delay_c = 0;
  gui.D_LFO_Wave_c = 0;
  gui.cop = 0;
  for (i = 0; i < 6; i++)
    {
      gui.D_Wave_c[i] = 0;
      gui.D_H_c[i] = 0;
      gui.D_HF_c[i] = 0;
      gui.D_OVol_c[i] = 0;
      gui.D_pLFO_c[i] = 0;
      gui.D_Attack_c[i] = 0;
      gui.D_Decay_c[i] = 0;
      gui.D_Sustain_c[i] = 0;
      gui.D_Release_c[i] = 0;
    }

  gui.D_Ftype_c = 0;
  gui.D_Fgain_c = 0;
  gui.D_Fcutoff_c = 0;
  gui.D_Fq_c = 0;
  gui.D_FLFO_c = 0;
  gui.D_FADSR_c = 0;
  gui.D_Fstages_c = 0;
  gui.D_Fvelocity_c = 0;
  gui.D_Algo_c = 0;


  New ();
  New_Bank ();
  char *url = argv[1];
  char *host = lo_url_get_hostname (url);
  char *port = lo_url_get_port (url);
  char *path = lo_url_get_path (url);
  char *label = argv[3];
  char *temp;


  pthread_create (&thr1, NULL, thread1, NULL);
  gui.ui_win->copy_label (argv[3]);
  gui.ui_win->show ();

  sprintf (gui.uBankFilename, "%s/Default.goomf", DATADIR);
  loadOK = loadbank (gui.uBankFilename);




  osc_server = 0;
  m_host = lo_address_new (host, port);

  osc_configure_path = osc_build_path (path, "/configure");
  osc_control_path = osc_build_path (path, "/control");
  osc_exiting_path = osc_build_path (path, "/exiting");
  osc_hide_path = osc_build_path (path, "/hide");
  osc_midi_path = osc_build_path (path, "/midi");
  osc_program_path = osc_build_path (path, "/program");
  osc_quit_path = osc_build_path (path, "/quit");
  osc_show_path = osc_build_path (path, "/show");
  osc_update_path = osc_build_path (path, "/update");

  osc_server = lo_server_new (NULL, osc_error);

  lo_server_add_method (osc_server, osc_configure_path, "ss",
			configure_handler, NULL);
  lo_server_add_method (osc_server, osc_control_path, "if", control_handler,
			NULL);
  lo_server_add_method (osc_server, osc_hide_path, "", hide_handler, NULL);
  lo_server_add_method (osc_server, osc_program_path, "ii", program_handler,
			NULL);
  lo_server_add_method (osc_server, osc_quit_path, "", quit_handler, NULL);
  lo_server_add_method (osc_server, osc_exiting_path, "", exiting_handler,
			NULL);
  lo_server_add_method (osc_server, osc_show_path, "", show_handler, NULL);
  lo_server_add_method (osc_server, NULL, NULL, debug_handler, NULL);

  temp = lo_server_get_url (osc_server);
  myurl = osc_build_path (temp, (strlen (path) > 1 ? path + 1 : path));
  lo_send (m_host, osc_update_path, "s", myurl);
  sleep (2);
  gui.d_osc_label->copy_label (myurl);
  gui.d_osc_label->redraw ();
  if (!loadOK)
    Send_laristra ();


  lo_fd = lo_server_get_socket_fd (osc_server);

  if (lo_fd > 0)
    {

      do
	{
	  FD_ZERO (&rfds);
	  FD_SET (lo_fd, &rfds);

	  retval = select (lo_fd + 1, &rfds, NULL, NULL, NULL);

	  if (retval == -1)
	    {
	      printf ("select() error\n");
	      exit (1);

	    }
	  else if (retval > 0)

	    {

	      if (FD_ISSET (0, &rfds))
		{
		  read_stdin ();
		}
	      if (FD_ISSET (lo_fd, &rfds))
		{
		  lo_server_recv_noblock (osc_server, 0);
		}
	    }


	}
      while (!done);

    }
  else
    {

      do
	{
	  FD_ZERO (&rfds);
	  FD_SET (0, &rfds);
	  tv.tv_sec = 0;
	  tv.tv_usec = 10000;

	  retval = select (1, &rfds, NULL, NULL, &tv);

	  if (retval == -1)
	    {

	      printf ("select() error\n");
	      exit (1);

	    }
	  else if (retval > 0 && FD_ISSET (0, &rfds))
	    {

	      read_stdin ();

	    }

	  lo_server_recv_noblock (osc_server, 0);



	}
      while (!done);

    }





  free (host);
  free (port);
  free (path);
  free (osc_configure_path);
  free (osc_control_path);
  free (osc_exiting_path);
  free (osc_hide_path);
  free (osc_midi_path);
  free (osc_program_path);
  free (osc_quit_path);
  free (osc_show_path);
  free (osc_update_path);
  free (myurl);


  return (0);





}

void
read_stdin (void)
{
  char buf[256];
  int len = read (0, buf, 256);
  if (len > 0)
    {
      printf ("stdin: ");
      fwrite (buf, len, 1, stdout);
      printf ("\n");
      fflush (stdout);
    }
}



void
New_Bank ()
{
  int i, j;
  for (j = 0; j < 80; j++)
    {

      bzero (Banco[j].Name, sizeof (Banco[j].Name));
      sprintf (Banco[j].Name, "");
      Banco[j].master_volume = 0.5f;
      Banco[j].tune = 0.0f;
      Banco[j].portamento = 0.0f;
      Banco[j].LFO_Frequency = 0.0f;
      Banco[j].LFO_Delay = 0.0f;
      Banco[j].LFO_Wave = 1;
      Banco[j].LFO_Volume = 0.0f;


      for (i = 0; i < 6; i++)
	{
	  Banco[j].H[i] = 0;
	  Banco[j].HF[i] = 0.0f;
	  Banco[j].Ovol[i] = 0.0f;
	  Banco[j].wave[i] = 1;
	  Banco[j].attack[i] = 0.01f;
	  Banco[j].decay[i] = 0.5f;
	  Banco[j].sustain[i] = 0.5f;
	  Banco[j].release[i] = 0.01;
	  Banco[j].pLFO[i] = 0.0f;
	}

      Banco[j].Ftype = 0;
      Banco[j].Fgain = 0.5f;
      Banco[j].Fcutoff = 5020.5f;
      Banco[j].Fq = 0.0;
      Banco[j].FLFO = 0.0;
      Banco[j].FADSR = 0;
      Banco[j].Fstages = 1;
      Banco[j].Fvelocity = 0;
      Banco[j].algo = 1;


    }

};


void
New ()
{
  int i;

  bzero (Name, sizeof (Name));
  sprintf (Name, "");
  master_volume = 0.5f;
  tune = 0.0f;
  portamento = 0.0f;
  LFO_Frequency = 0.0f;
  LFO_Delay = 0.0f;
  LFO_Wave = 1;
  LFO_Volume = 0.0f;


  for (i = 0; i < 6; i++)
    {
      H[i] = 0;
      HF[i] = 0.0f;
      Ovol[i] = 0.0f;
      wave[i] = 1;
      attack[i] = 0.01f;
      decay[i] = 0.5f;
      sustain[i] = 0.5f;
      release[i] = 0.01;
      pLFO[i] = 0.0f;
    }

  Ftype = 0;
  Fgain = 0.5f;
  Fcutoff = 5020.5f;
  Fq = 0.0;
  FLFO = 0.0;
  FADSR = 0;
  Fstages = 1;
  Fvelocity = 0;
  algo = 1;




};


void
Put_Combi (int j)
{

  int i;

  strcpy (Name, Banco[j].Name);

  master_volume = Banco[j].master_volume;
  tune = Banco[j].tune;
  portamento = Banco[j].portamento;
  LFO_Frequency = Banco[j].LFO_Frequency;
  LFO_Delay = Banco[j].LFO_Delay;
  LFO_Wave = Banco[j].LFO_Wave;
  LFO_Volume = Banco[j].LFO_Volume;

  for (i = 0; i < 6; i++)
    {
      H[i] = Banco[j].H[i];
      HF[i] = Banco[j].HF[i];
      Ovol[i] = Banco[j].Ovol[i];
      wave[i] = Banco[j].wave[i];
      attack[i] = Banco[j].attack[i];
      decay[i] = Banco[j].decay[i];
      sustain[i] = Banco[j].sustain[i];
      release[i] = Banco[j].release[i];
      pLFO[i] = Banco[j].pLFO[i];
    }

  Ftype = Banco[j].Ftype;
  Fgain = Banco[j].Fgain;
  Fcutoff = Banco[j].Fcutoff;
  Fq = Banco[j].Fq;
  FLFO = Banco[j].FLFO;
  FADSR = Banco[j].FADSR;
  Fstages = Banco[j].Fstages;
  Fvelocity = Banco[j].Fvelocity;
  algo = Banco[j].algo;


};


void
preset_to_bank (int j)
{
  int i;

  strcpy (Banco[j].Name, Name);
  Banco[j].master_volume = master_volume;
  Banco[j].tune = tune;
  Banco[j].portamento = portamento;
  Banco[j].LFO_Frequency = LFO_Frequency;
  Banco[j].LFO_Delay = LFO_Delay;
  Banco[j].LFO_Wave = LFO_Wave;
  Banco[j].LFO_Volume = LFO_Volume;

  for (i = 0; i < 6; i++)
    {
      Banco[j].H[i] = H[i];
      Banco[j].HF[i] = HF[i];
      Banco[j].Ovol[i] = Ovol[i];
      Banco[j].wave[i] = wave[i];
      Banco[j].attack[i] = attack[i];
      Banco[j].decay[i] = decay[i];
      Banco[j].sustain[i] = sustain[i];
      Banco[j].release[i] = release[i];
      Banco[j].pLFO[i] = pLFO[i];
    }


  Banco[j].Ftype = Ftype;
  Banco[j].Fgain = Fgain;
  Banco[j].Fcutoff = Fcutoff;
  Banco[j].Fq = Fq;
  Banco[j].FLFO = FLFO;
  Banco[j].FADSR = FADSR;
  Banco[j].Fstages = Fstages;
  Banco[j].Fvelocity = Fvelocity;
  Banco[j].algo = algo;



}

int
savebank (const char *filename)
{

  FILE *fn;
  char buf[2048];
  int i, j;
  fn = fopen (filename, "w");

  for (j = 0; j < 80; j++)
    {
      bzero (buf, sizeof (buf));
      sprintf (buf, "%s\n", Banco[j].Name);
      fputs (buf, fn);

      bzero (buf, sizeof (buf));
      sprintf (buf, "%f,%f,%f,%d\n", Banco[j].master_volume, Banco[j].tune,
	       Banco[j].portamento, Banco[j].algo);
      fputs (buf, fn);

      bzero (buf, sizeof (buf));
      sprintf (buf, "%f,%f,%f,%d\n", Banco[j].LFO_Volume,
	       Banco[j].LFO_Frequency, Banco[j].LFO_Delay, Banco[j].LFO_Wave);
      fputs (buf, fn);

      for (i = 0; i < 6; i++)
	{
	  bzero (buf, sizeof (buf));
	  sprintf (buf, "%d,%d,%f,%f,%f,%f,%f,%f,%f\n", Banco[j].wave[i],
		   Banco[j].H[i], Banco[j].HF[i], Banco[j].Ovol[i],
		   Banco[j].attack[i], Banco[j].decay[i], Banco[j].sustain[i],
		   Banco[j].release[i], Banco[j].pLFO[i]);
	  fputs (buf, fn);
	}

      bzero (buf, sizeof (buf));
      sprintf (buf, "%d,%f,%f,%f\n", Banco[j].Ftype, Banco[j].Fgain,
	       Banco[j].Fcutoff, Banco[j].Fq);
      fputs (buf, fn);

      bzero (buf, sizeof (buf));
      sprintf (buf, "%f,%d,%d,%d\n", Banco[j].FLFO, Banco[j].FADSR,
	       Banco[j].Fstages, Banco[j].Fvelocity);
      fputs (buf, fn);



    }

  return 0;


}



int
loadbank (const char *filename)
{

  FILE *fn;
  char buf[2048];
  int i, j;

  if ((fn = fopen (filename, "r")) == NULL)
    {
      fprintf (stderr, "File not found\n");
      return 1;
    }

  New_Bank ();

  for (j = 0; j < 80; j++)
    {

      bzero (buf, sizeof (buf));
      fgets (buf, sizeof buf, fn);
      for (i = 0; i < 64; i++)
	if (buf[i] > 20)
	  Banco[j].Name[i] = buf[i];

      bzero (buf, sizeof (buf));
      fgets (buf, sizeof buf, fn);
      sscanf (buf, "%f,%f,%f,%d", &Banco[j].master_volume, &Banco[j].tune,
	      &Banco[j].portamento, &Banco[j].algo);

      bzero (buf, sizeof (buf));
      fgets (buf, sizeof buf, fn);
      sscanf (buf, "%f,%f,%f,%d", &Banco[j].LFO_Volume,
	      &Banco[j].LFO_Frequency, &Banco[j].LFO_Delay,
	      &Banco[j].LFO_Wave);

      for (i = 0; i < 6; i++)
	{
	  bzero (buf, sizeof (buf));
	  fgets (buf, sizeof buf, fn);
	  sscanf (buf, "%d,%d,%f,%f,%f,%f,%f,%f,%f", &Banco[j].wave[i],
		  &Banco[j].H[i], &Banco[j].HF[i], &Banco[j].Ovol[i],
		  &Banco[j].attack[i], &Banco[j].decay[i],
		  &Banco[j].sustain[i], &Banco[j].release[i],
		  &Banco[j].pLFO[i]);
	}


      bzero (buf, sizeof (buf));
      fgets (buf, sizeof buf, fn);
      sscanf (buf, "%d,%f,%f,%f\n", &Banco[j].Ftype, &Banco[j].Fgain,
	      &Banco[j].Fcutoff, &Banco[j].Fq);

      bzero (buf, sizeof (buf));
      fgets (buf, sizeof buf, fn);
      sscanf (buf, "%f,%d,%d,%d\n", &Banco[j].FLFO, &Banco[j].FADSR,
	      &Banco[j].Fstages, &Banco[j].Fvelocity);




    }

  fclose (fn);
  Haz_laristra ();
  return 0;
};


void
Update_Operator_Widgets (int op)
{

  update_widgets (9, wave[op]);
  update_widgets (10, H[op]);
  update_widgets (11, HF[op]);
  update_widgets (12, Ovol[op]);
  update_widgets (13, attack[op]);
  update_widgets (14, decay[op]);
  update_widgets (15, sustain[op]);
  update_widgets (16, release[op]);
  update_widgets (17, pLFO[op]);

}


void
Update_Main_Widgets ()
{

  update_widgets (2, master_volume);
  update_widgets (3, tune);
  update_widgets (4, portamento);
  update_widgets (5, LFO_Volume);
  update_widgets (6, LFO_Frequency);
  update_widgets (7, LFO_Delay);
  update_widgets (8, LFO_Wave);
  update_widgets (63, Ftype);
  update_widgets (64, Fgain);
  update_widgets (65, Fcutoff);
  update_widgets (66, Fq);
  update_widgets (67, FLFO);
  update_widgets (68, FADSR);
  update_widgets (69, Fstages);
  update_widgets (70, Fvelocity);
  update_widgets (71, algo);


}

void
Send_Values ()
{
  int i;

  lo_send (m_host, osc_control_path, "if", 2, master_volume);
  lo_send (m_host, osc_control_path, "if", 3, tune);
  lo_send (m_host, osc_control_path, "if", 4, portamento);
  lo_send (m_host, osc_control_path, "if", 5, LFO_Volume);
  lo_send (m_host, osc_control_path, "if", 6, LFO_Frequency);
  lo_send (m_host, osc_control_path, "if", 7, LFO_Delay);
  lo_send (m_host, osc_control_path, "if", 8, (float) LFO_Wave);


  for (i = 0; i < 6; i++)
    {

      lo_send (m_host, osc_control_path, "if", 9 + i, (float) wave[i]);
      lo_send (m_host, osc_control_path, "if", 15 + i, (float) H[i]);
      lo_send (m_host, osc_control_path, "if", 21 + i, HF[i]);
      lo_send (m_host, osc_control_path, "if", 27 + i, Ovol[i]);
      lo_send (m_host, osc_control_path, "if", 33 + i, attack[i]);
      lo_send (m_host, osc_control_path, "if", 39 + i, decay[i]);
      lo_send (m_host, osc_control_path, "if", 45 + i, sustain[i]);
      lo_send (m_host, osc_control_path, "if", 51 + i, release[i]);
      lo_send (m_host, osc_control_path, "if", 57 + i, pLFO[i]);
    }

  lo_send (m_host, osc_control_path, "if", 63, (float) Ftype);
  lo_send (m_host, osc_control_path, "if", 64, Fgain);
  lo_send (m_host, osc_control_path, "if", 65, Fcutoff);
  lo_send (m_host, osc_control_path, "if", 66, Fq);
  lo_send (m_host, osc_control_path, "if", 67, FLFO);
  lo_send (m_host, osc_control_path, "if", 68, (float) FADSR);
  lo_send (m_host, osc_control_path, "if", 69, (float) Fstages);
  lo_send (m_host, osc_control_path, "if", 70, (float) Fvelocity);
  lo_send (m_host, osc_control_path, "if", 71, (float) algo);

}


void
preset_click (Fl_Button * o, void *)
{

  int ok;

  char temp2[128];
  long long kk = (long long) o->user_data ();
  int num = (int) kk;
  int tecla = Fl::event_key ();

  if (Fl::event_button () == 3)
    {

      Fl_Widget *w = gui.ob->child (num);


      if (strlen (Banco[num].Name) > 0)
	{
	  Fl_Widget *m = fl_message_icon ();
	  m->parent ()->copy_label ("goomf");
	  sprintf (temp2, "Overwrite? \"%s\"", w->label ());
	  ok = fl_choice (temp2, "No", "Yes", NULL);
	  if (!ok)
	    return;
	}

      strcpy (Name, gui.Pname->value ());
      preset_to_bank (num);
      w->copy_label (gui.Pname->value ());
      Haz_laristra ();
      Send_laristra ();
    }



  if ((Fl::event_button () == 1) || tecla == 32)
    {
      gui.PresetSelect->value (num);
      Put_Combi (num);
      gui.ready = 2;
    }

}


void
make_window_banks ()
{

  int i, j, x, y, num;
  int elw, elh;

  elw = 176 * gui.BankWindow->w () / 800;
  elh = 24 * gui.BankWindow->h () / 600;



  gui.ob->begin ();

  x = 40;
  y = 10;
  num = 0;

  for (i = 1; i < 21; i++)
    {
      y += elh + 2;

      for (j = 1; j < 5; j++)
	{

	  Fl_Button *butX =
	    new Fl_Button (x + 1, y + 1, elw, elh, Banco[num].Name);
	  butX->type (0);
	  butX->color (FL_BLUE);
	  butX->box (FL_FLAT_BOX);
	  butX->selection_color (FL_RED);
	  butX->labelsize (14);
	  butX->align (68 | FL_ALIGN_INSIDE);
	  butX->user_data ((void *) (num));
	  butX->value (0);
	  butX->callback ((Fl_Callback *) preset_click);
	  gui.ob->add (butX);

	  x += elw + 4;
	  num++;

	}
      x = 40;

    }


  gui.ob->end ();

  gui.made = 1;

}

void
Put_Loaded_Bank ()
{
  int k = 0;

  for (int t = 0; t < gui.ob->children (); t++)
    {
      Fl_Widget *w = gui.ob->child (t);
      w->copy_label (Banco[k].Name);
      k++;

    }

}

void
Haz_laristra ()
{

  int i;
  bzero (laristra, sizeof (laristra));
  for (i = 0; i < 80; i++)
    {
      if (i == 0)
	sprintf (laristra, "%s", Banco[i].Name);
      else
	sprintf (laristra, "%s,%s", laristra, Banco[i].Name);
    }


}


void
Send_laristra ()
{
  lo_send (m_host, osc_configure_path, "ss", "names", laristra);
}


void
set_FilterType (int value)
{
  switch (value)
    {
    case 0:
      gui.Flabel->copy_label ("LPF 1 pole");
      break;
    case 1:
      gui.Flabel->copy_label ("HPF 1 pole");
      break;
    case 2:
      gui.Flabel->copy_label ("LPF 2 poles");
      break;
    case 3:
      gui.Flabel->copy_label ("HPF 2 poles");
      break;
    case 4:
      gui.Flabel->copy_label ("BPF 2 poles");
      break;
    case 5:
      gui.Flabel->copy_label ("NOTCH 2 poles");
      break;
    case 6:
      gui.Flabel->copy_label ("PEAK 2 poles");
      break;
    case 7:
      gui.Flabel->copy_label ("Low Shelf 2 poles");
      break;
    case 8:
      gui.Flabel->copy_label ("High Shelf 2 poles");
      break;
    }

}

