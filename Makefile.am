SUBDIRS = libmonograph

AM_CPPFLAGS = -I$(srcdir)/libmonograph/src

bin_PROGRAMS = 1g
1g_SOURCES = src/main.c \
			 src/options.h src/options.c
1g_LDADD = libmonograph/libmonograph.a

EXTRA_DIST = LICENSE README.md

