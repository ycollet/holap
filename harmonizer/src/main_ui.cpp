/*
  holharm - a smart harmonizer DSSI plugin software

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

      if (gui.D_Wet_c)
	{
	  lo_send (m_host, osc_control_path, "if", 4, gui.D_Wet->value ());
	  gui.D_Wet_c = 0;
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

      if (gui.D_IntL_c)
	{
	  lo_send (m_host, osc_control_path, "if", 7, gui.D_IntL->value ());
	  gui.D_IntL_c = 0;
	}
      if (gui.D_IntR_c)
	{
	  lo_send (m_host, osc_control_path, "if", 8, gui.D_IntR->value ());
	  gui.D_IntR_c = 0;
	}
      if (gui.D_IntRL_c)
	{
	  gui.D_IntR->value (gui.D_IntRL->value());
	  gui.D_IntL->value(gui.D_IntRL->value());
	  lo_send (m_host, osc_control_path, "if", 7, gui.D_IntL->value ());
	  lo_send (m_host, osc_control_path, "if", 8, gui.D_IntR->value ());
	  gui.D_IntRL_c = 0;
	}


      if (gui.D_Type_c)
	{
	  lo_send (m_host, osc_control_path, "if", 9, (float) gui.D_Type->value ());
	  update_mode((int)gui.D_Type->value());
	  gui.D_Type_c = 0;
	}
   
      if (gui.D_Note_c)
	{
	  lo_send (m_host, osc_control_path, "if", 10, (float) gui.D_Note->value ());
	  gui.D_Note_c = 0;
	  update_sel_chord((int)gui.D_Note->value(), (int)gui.D_ChType->value());
	}
      if (gui.D_ChType_c)
	{
	  lo_send (m_host, osc_control_path, "if", 11, (float) gui.D_ChType->value ());
	  gui.D_ChType_c = 0;
          update_sel_chord((int)gui.D_Note->value(), (int)gui.D_ChType->value());
	}
     
      if (gui.D_recocount_c)
        { 
          lo_send (m_host, osc_control_path, "if", 12, (float) gui.D_recocount->value ());
                    gui.D_recocount_c = 0;
                    
        }

      if (gui.Pexit)
        {
        gui.Pexit=0;
        lo_send (m_host, osc_exiting_path, "");
        exit(0);        
        }


    }


};


void
initchords()
{


int i;

  bzero (ChN, sizeof (ChN));

  sprintf(ChN[0].Nom," ");
  sprintf(ChN[1].Nom,"6"); 
  sprintf(ChN[2].Nom,"Maj7");
  sprintf(ChN[3].Nom,"lyd"); 
  sprintf(ChN[4].Nom,"Maj(9)");
  sprintf(ChN[5].Nom,"Maj7(9)"); 
  sprintf(ChN[6].Nom,"6(9)");
  sprintf(ChN[7].Nom,"+"); 
  sprintf(ChN[8].Nom,"m");
  sprintf(ChN[9].Nom,"m6"); 
  sprintf(ChN[10].Nom,"m7");
  sprintf(ChN[11].Nom,"m7(b5)"); 
  sprintf(ChN[12].Nom,"m9");
  sprintf(ChN[13].Nom,"m7(9)"); 
  sprintf(ChN[14].Nom,"m7(11)");
  sprintf(ChN[15].Nom,"m(Maj7)"); 
  sprintf(ChN[16].Nom,"m(Maj7)(9)");
  sprintf(ChN[17].Nom,"dim"); 
  sprintf(ChN[18].Nom,"dim7");
  sprintf(ChN[19].Nom,"7"); 
  sprintf(ChN[20].Nom,"7(Sus4)");
  sprintf(ChN[21].Nom,"7(b5)"); 
  sprintf(ChN[22].Nom,"7(9)");
  sprintf(ChN[23].Nom,"7(#11)"); 
  sprintf(ChN[24].Nom,"7(13)");
  sprintf(ChN[25].Nom,"7(b9)"); 
  sprintf(ChN[26].Nom,"7(b13)");
  sprintf(ChN[27].Nom,"7(#9)"); 
  sprintf(ChN[28].Nom,"+Maj7");
  sprintf(ChN[29].Nom,"+7"); 
  sprintf(ChN[30].Nom,"1+8");
  sprintf(ChN[31].Nom,"1+5"); 
  sprintf(ChN[32].Nom,"(Sus4)");
  sprintf(ChN[33].Nom,"2"); 
  
  for (i = 0; i <= 33; i++)
    {
      ChN[i].tipo = i;
    }

  ChN[1].num = 1;
  ChN[6].num = 1;
  ChN[9].num = 1;
  ChN[32].num = 1;

  sprintf(NC[0].Nom,"C");
  sprintf(NC[1].Nom,"Db");
  sprintf(NC[2].Nom,"D");
  sprintf(NC[3].Nom,"Eb");
  sprintf(NC[4].Nom,"E");
  sprintf(NC[5].Nom,"F");
  sprintf(NC[6].Nom,"Gb");
  sprintf(NC[7].Nom,"G");
  sprintf(NC[8].Nom,"Ab");
  sprintf(NC[9].Nom,"A");
  sprintf(NC[10].Nom,"Bb");
  sprintf(NC[11].Nom,"B");
  
}

void
add_items()
{
int i;

  gui.D_Type->add("Normal");
  gui.D_Type->add("Selected");
  gui.D_Type->add("MIDI");
  
  for (i=0; i<=11; i++) gui.D_Note->add(NC[i].Nom);
  for (i=0; i<=33; i++) gui.D_ChType->add(ChN[i].Nom);
    
}

void
update_widgets (int port, float value)
{

  switch (port)
    {
    case 4:
      gui.D_Wet->value (value);
      break;
    case 5:
      gui.D_Vol->value (value);
      break;
    case 6:
      gui.D_Pan->value (value);
      break;
    case 7:
      gui.D_IntL->value (value);
      break;
    case 8:
      gui.D_IntR->value (value);  
      break;
    case 9:
      gui.D_Type->value (value);
      update_mode((int)gui.D_Type->value());
      break;
    case 10:
      gui.D_Note->value (value);
      update_sel_chord((int)gui.D_Note->value(),(int) gui.D_ChType->value());
      break;
    case 11:
      gui.D_ChType->value (value); 
      update_sel_chord((int)gui.D_Note->value(),(int) gui.D_ChType->value());     
      break; 

    }


}

void
update_sel_chord(int note, int type)
{

 char tmp[32];

 bzero(tmp,sizeof(tmp));
 sprintf(tmp,"%s%s",NC[note].Nom,ChN[type].Nom);
 gui.SelectedChord->copy_label(tmp);

} 
  
void
update_mode(int mode)
{

  switch(mode)
  {
    case 0:
    gui.ShowMode->copy_label("Normal");
    break;

    case 1:
    gui.ShowMode->copy_label("Selected");
    break;

    case 2:
    gui.ShowMode->copy_label("Midi");
    break;

   }
}   

void
ReadConfig()
{
  FILE *fs;
  char temp[32];
  char filename[128];
  sprintf (filename, "/home/%s/.holharm", getenv ("USER"));
     if ((fs = fopen (filename, "r")) != NULL)
         {
           bzero (temp, sizeof (temp));
           fgets (temp, sizeof temp, fs);
           sscanf (temp, "%d", &Quality);
           fclose (fs);
         }
                                     
 gui.Qadjust(Quality); 

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
  initchords();
  add_items();
  funcion = 0;
  gui.Pexitprogram = 0;
  gui.D_Wet_c = 0;  gui.D_Vol_c = 0;
  gui.D_Pan_c = 0;  gui.D_IntL_c = 0;
  gui.D_IntR_c = 0; gui.D_Type_c = 0;
  gui.D_Note_c = 0; gui.D_ChType_c = 0;

  
  char *url = argv[1];
  char *host = lo_url_get_hostname (url);
  char *port = lo_url_get_port (url);
  char *path = lo_url_get_path (url);
  char *label = argv[3];
  char *temp;


  pthread_create (&thr1, NULL, thread1, NULL);

  gui.ui_win->copy_label (argv[3]);
  
  
  gui.ui_win->show ();

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

  ReadConfig();
  
  
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

