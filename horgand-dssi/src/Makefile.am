ui_PROGRAMS = horgand_fltk

uidir = $(libdir)/dssi/horgand

plugindir = $(libdir)/dssi

plugin_LTLIBRARIES = horgand.la


horgand_fltk_SOURCES = main_ui.cpp \
	main_ui.h \
	horgand_ui.cxx \
	horgand128.xpm \
	horgand_ui.h 



horgand_fltk_LDADD =  -lfltk -lfltk_images -lpthread

horgand_fltk_CXXFLAGS = "-I/usr/include/freetype2 -D_THREAD_SAFE -D_REENTRANT"

horgand_fltk_CFLAGS = "-D_THREAD_SAFE -D_REENTRANT"

horgand_la_SOURCES = horgand.c \
        organ.c \
	organDSPEffects.c \
	organvarios.c \
	organFileIO.c \
        Holrgan.h 

horgand_la_LIBADD = -lm -lpthread

horgand_la_LDFLAGS = -module -avoid-version 

install-data-hook:
	rm $(plugindir)/horgand.la
	rm $(plugindir)/horgand.a
	
        