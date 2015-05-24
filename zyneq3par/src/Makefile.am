
plugin_LTLIBRARIES = zyneq3par.la

plugindir = $(libdir)/ladspa

zyneq3par_la_SOURCES = zyneq3par.c \
	eq.c \
	analogfilter.c \
	eq.h

AM_LDFLAGS = -module -avoid-version 

zyneq3par_la_LDFLAGS = -avoid-version -module -shared -export-symbols-regex "ladspa_descriptor"

ACLOCAL_AMFLAGS = -I m4

install-data-hook:

	rm $(plugindir)/zyneq3par.la
	
	


  

