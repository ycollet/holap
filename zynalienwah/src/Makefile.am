plugin_LTLIBRARIES = zynalienwah.la

plugindir = $(libdir)/ladspa

zynalienwah_la_SOURCES = zynalienwah.c \
	alienwah.c \
	effectlfo.c \
	alienwah.h

AM_LDFLAGS = -module -avoid-version

zynalienwah_la_LDFLAGS = -avoid-version -module -shared -export-symbols-regex "ladspa_descriptor"

ACLOCAL_AMFLAGS = -I m4

install-data-hook:

	rm $(plugindir)/zynalienwah.la
	
	


  

