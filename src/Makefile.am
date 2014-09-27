SUBDIRS = libmoxie .
ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/include

bin_PROGRAMS = run_moxie
run_moxie_LDADD = libmoxie/libmoxie.la
run_moxie_SOURCES = run_moxie.c moxie_compiler_sample.c

