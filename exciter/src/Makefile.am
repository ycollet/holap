ui_PROGRAMS = exciter_fltk

uidir = $(libdir)/dssi/exciter

plugindir = $(libdir)/dssi

plugin_LTLIBRARIES = exciter.la

exciter_fltk_SOURCES = main_ui.cpp \
	main_ui.h \
	exciter_ui.cxx \
	exciter_ui.h 

exciter_fltk_LDADD = -lfltk -lfltk_images -lpthread 

exciter_fltk_CXXFLAGS = "-I/usr/include/freetype2 -D_THREAD_SAFE -D_REENTRANT"
 
exciter_fltk_CFLAGS = "-D_THREAD_SAFE -D_REENTRANT"

  
exciter_la_SOURCES =  exciter.c \
	recchord.c \
	analogfilter.c \
	harmonic.c \
	exciter.h
  
exciter_la_LDFLAGS = -module -avoid-version -export-symbols-regex "(ladspa_|dssi_)descriptor"


install-data-hook:
	rm $(plugindir)/exciter.la
	rm $(plugindir)/exciter.a
	