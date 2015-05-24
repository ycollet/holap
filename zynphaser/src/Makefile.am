plugin_LTLIBRARIES = zynphaser.la

plugindir = $(libdir)/ladspa

zynphaser_la_SOURCES = zynphaser.c \
	phaser.c \
	effectlfo.c \
	phaser.h

AM_LDFLAGS = -module -avoid-version 

zynphaser_la_LDFLAGS = -avoid-version -module -shared -export-symbols-regex "ladspa_descriptor"

ACLOCAL_AMFLAGS = -I m4

install-data-hook:

	rm $(plugindir)/zynphaser.la
	
	


  

