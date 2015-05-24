/*
  exciter - a MIDI helped exciter plugin software

  main_ui.cpp  -  Main dssi GUI file of the plugin
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

#include <dssi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <lo/lo.h>
#include "main_ui.h"


pthread_t thr1;

void *
thread1 (void *arg)
{

  while (gui.Pexitprogram == 0)
    {
      Fl::wait ();

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

      if (gui.D_Dry_c)
	{
	  lo_send (m_host, osc_control_path, "if", 4, gui.D_Dry->value ());
	  gui.D_Dry_c = 0;
	}

      if (gui.D_Vol_c)
	{
	  lo_send (m_host, osc_control_path, "if", 5, gui.D_Vol->value ());
	  gui.D_Vol_c = 0;
	}
      
      if (gui.D_Pan_c)
	{
	  lo_send (m_host, osc_control_path, "if", 6, gui.D_Pan->value ());
	  gui.D_Pan_c = 0;
	}

      if (gui.D_Mag1_c)
	{
	  lo_send (m_host, osc_control_path, "if", 7, gui.D_Mag1->value ());
	  gui.D_Mag1_c = 0;
	}
      if (gui.D_Mag2_c)
	{
	  lo_send (m_host, osc_control_path, "if", 8, gui.D_Mag2->value ());
	  gui.D_Mag2_c = 0;
	}
      if (gui.D_Mag3_c)
	{
	  lo_send (m_host, osc_control_path, "if", 9, gui.D_Mag3->value ());
	  gui.D_Mag3_c = 0;
	}
      if (gui.D_Mag4_c)
	{
	  lo_send (m_host, osc_control_path, "if", 10, gui.D_Mag4->value ());
	  gui.D_Mag4_c = 0;
	}
      if (gui.D_Mag5_c)
	{
	  lo_send (m_host, osc_control_path, "if", 11, gui.D_Mag5->value ());
	  gui.D_Mag5_c = 0;
	}
      if (gui.D_Mag6_c)
	{
	  lo_send (m_host, osc_control_path, "if", 12, gui.D_Mag6->value ());
	  gui.D_Mag6_c = 0;
	}
      if (gui.D_Mag7_c)
	{
	  lo_send (m_host, osc_control_path, "if", 13, gui.D_Mag7->value ());
	  gui.D_Mag7_c = 0;
	}
      if (gui.D_Mag8_c)
	{
	  lo_send (m_host, osc_control_path, "if", 14, gui.D_Mag8->value ());
	  gui.D_Mag8_c = 0;
	}
      if (gui.D_Mag9_c)
	{
	  lo_send (m_host, osc_control_path, "if", 15, gui.D_Mag9->value ());
	  gui.D_Mag9_c = 0;
	}
      if (gui.D_Mag10_c)
	{
	  lo_send (m_host, osc_control_path, "if", 16, gui.D_Mag10->value ());
	  gui.D_Mag10_c = 0;
	}
      if (gui.D_Freq_c)
	{
	  lo_send (m_host, osc_control_path, "if", 17, gui.D_Freq->value ());
	  gui.D_Freq_c = 0;
	}

      if(gui.Pexit)
        {
          gui.Pexit=0; 
          lo_send (m_host, osc_exiting_path, "");
          exit(0);
        }  



    }


};


void
update_widgets (int port, float value)
{

  switch (port)
    {
    case 4:
      gui.D_Dry->value (value);
      break;
    case 5:
      gui.D_Vol->value (value);
      break;
    case 6:
      gui.D_Pan->value (value);
      break;
    case 7:
      gui.D_Mag1->value (value);
      break;
    case 8:
      gui.D_Mag2->value (value);
      break;
    case 9:
      gui.D_Mag3->value (value);
      break;
    case 10:
      gui.D_Mag4->value (value);
      break;
    case 11:
      gui.D_Mag5->value (value);
      break;
    case 12:
      gui.D_Mag6->value (value);
      break;
    case 13:
      gui.D_Mag7->value (value);
      break;
    case 14:
      gui.D_Mag8->value (value);
      break;
    case 15:
      gui.D_Mag9->value (value);
      break;
    case 16:
      gui.D_Mag10->value (value);
      break;
    case 17:
      gui.D_Freq->value (value);
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
      printf ("out of memory!\n");
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

   if (!strcmp (key, "schord"))
       {
        gui.ShowChord->copy_label(value) ;
         
       }
                 
  return 0;
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
  lo_send (m_host, osc_exiting_path, "");
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

  update_widgets (port, value);


  return 0;
}


int
program_handler (const char *path, const char *types, lo_arg ** argv,
		 int argc, lo_message msg, void *user_data)
{
  int bank, program;

  if (argc < 2)
    {
      printf (" error: too few arguments to osc_program_handler\n");
      return 1;
    }

  bank = argv[0]->i;
  program = argv[1]->i;


  printf
    ("osc_program_handler: received program change, bank %d, program %d\n",
     bank, program);

  return 0;
}


int
main (int argc, char **argv)
{

  int lo_fd;
  fd_set rfds;
  struct timeval tv;
  int retval;
  int done = 0;
  funcion = 0;
  gui.Pexitprogram = 0;
  gui.D_Dry_c = 0;  gui.D_Mag3_c = 0;
  gui.D_Pan_c = 0;  gui.D_Mag4_c = 0;
  gui.D_Mag1_c = 0; gui.D_Mag5_c = 0;
  gui.D_Mag2_c = 0; gui.D_Mag6_c = 0;
  gui.D_Mag7_c = 0; gui.D_Mag8_c = 0;
  gui.D_Mag9_c = 0; gui.D_Mag10_c = 0;
  gui.D_Freq_c = 0; 

  
  char *url = argv[1];
  char *host = lo_url_get_hostname (url);
  char *port = lo_url_get_port (url);
  char *path = lo_url_get_path (url);
  char *label = argv[3];
  char *temp;

  pthread_create (&thr1, NULL, thread1, NULL);


  gui.ui_win->copy_label (argv[3]);
  gui.ui_win->show();

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

  gui.d_osc_label->copy_label (myurl);

  lo_send (m_host, osc_update_path, "s", myurl);

  lo_send (m_host, osc_configure_path, "ss", "url",myurl);

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

