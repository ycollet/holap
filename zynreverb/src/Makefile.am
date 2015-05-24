
plugin_LTLIBRARIES = zynreverb.la

plugindir = $(libdir)/ladspa

zynreverb_la_SOURCES = zynreverb.c \
	reverb.c \
	analogfilter.c \
	reverb.h

AM_LDFLAGS = -module -avoid-version

zynreverb_la_LDFLAGS = -avoid-version -module -shared -export-symbols-regex "ladspa_descriptor"

ACLOCAL_AMFLAGS = -I m4

install-data-hook:

	rm $(plugindir)/zynreverb.la
	
	


  

