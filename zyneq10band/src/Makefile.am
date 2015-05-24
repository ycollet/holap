
plugin_LTLIBRARIES = zyneq10band.la

plugindir = $(libdir)/ladspa

zyneq10band_la_SOURCES = zyneq10band.c \
	eq10.c \
	analogfilter.c \
	eq10.h

AM_LDFLAGS = -module -avoid-version

zyneq10band_la_LDFLAGS = -avoid-version -module -shared -export-symbols-regex "ladspa_descriptor"

ACLOCAL_AMFLAGS = -I m4

install-data-hook:

	rm $(plugindir)/zyneq10band.la
	
	


  

