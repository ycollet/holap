ui_PROGRAMS = holharm_fltk

uidir = $(libdir)/dssi/holharm

plugindir = $(libdir)/dssi

plugin_LTLIBRARIES = holharm.la

holharm_fltk_SOURCES = main_ui.cpp \
	main_ui.h \
	holharm_ui.cxx \
	holharm_ui.h 

holharm_fltk_LDADD = -lfltk -lfltk_images -lpthread

holharm_fltk_CXXFLAGS = "-I/usr/include/freetype2 -D_THREAD_SAFE -D_REENTRANT"
 
holharm_fltk_CFLAGS = "-D_THREAD_SAFE -D_REENTRANT"

  
holharm_la_SOURCES =  holharm.c \
	recchord.c \
	recognizenote.c \
	smbPitchShift.c \
	harmonizer.c \
	harmonizer.h
  
holharm_la_LIBADD = -lm -lpthread 
  
holharm_la_LDFLAGS = -module -avoid-version -export-symbols-regex "(ladspa_|dssi_)descriptor"

install-data-hook:
	rm $(plugindir)/holharm.la
	rm $(plugindir)/holharm.a
	               



