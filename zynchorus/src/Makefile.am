plugin_LTLIBRARIES = zynchorus.la

plugindir = $(libdir)/ladspa

zynchorus_la_SOURCES = zynchorus.c \
	chorus.c \
	effectlfo.c \
	chorus.h

AM_LDFLAGS = -module -avoid-version

zynchorus_la_LDFLAGS = -avoid-version -module -shared -export-symbols-regex "ladspa_descriptor"

ACLOCAL_AMFLAGS = -I m4

install-data-hook:

	rm $(plugindir)/zynchorus.la
	


  

