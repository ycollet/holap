ui_PROGRAMS = goomf_fltk

uidir = $(libdir)/dssi/goomf

plugindir = $(libdir)/dssi

plugin_LTLIBRARIES = goomf.la


goomf_fltk_SOURCES = main_ui.cpp \
	main_ui.h \
	goomf_ui.cxx \
	goomf_ui.h 


goomf_fltk_LDADD =  -lfltk -lfltk_images 

goomf_fltk_CXXFLAGS = "-I/usr/include/freetype2 -D_THREAD_SAFE -D_REENTRANT"

goomf_fltk_CFLAGS = "-D_THREAD_SAFE -D_REENTRANT"

goomf_la_SOURCES = goomf.c \
        synth.c \
        analogfilter.c \
        goomf.h 

goomf_la_LDFLAGS = -module -avoid-version 

install-data-hook:
	rm $(plugindir)/goomf.la
	rm $(plugindir)/goomf.a
	
        