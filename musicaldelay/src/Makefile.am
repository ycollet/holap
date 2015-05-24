plugin_LTLIBRARIES = musicaldelay.la

plugindir = $(libdir)/ladspa

musicaldelay_la_SOURCES = musicaldelay.c \
	mdelay.c \
	mdelay.h
	

AM_LDFLAGS = -module -avoid-version 

musicaldelay_la_LDFLAGS = -avoid-version -module -shared -export-symbols-regex "ladspa_descriptor"

ACLOCAL_AMFLAGS = -I m4

install-data-hook:

	rm $(plugindir)/musicaldelay.la
	
	


  

