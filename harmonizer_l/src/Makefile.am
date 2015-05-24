plugindir = $(libdir)/ladspa

plugin_LTLIBRARIES = holharm_l.la

holharm_l_la_SOURCES =  holharm_l.c \
	recchord.c \
	recognizenote.c \
	smbPitchShift.c \
	harmonizer.c \
	harmonizer.h
  
AM_LDFLAGS = -module -avoid-version

holharm_l_la_LDFLAGS = -module -avoid-version -shared -export-symbols-regex "(ladspa_|dssi_)descriptor"

ACLOCAL_AMFLAGS = -I m4

install-data-hook:
	rm $(plugindir)/holharm_l.la
        

