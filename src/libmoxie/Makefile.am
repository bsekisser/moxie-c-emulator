ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/../include

noinst_LTLIBRARIES = libmoxie-compiler.la libmoxie-trace.la
lib_LTLIBRARIES = libmoxie.la

libmoxie_compiler_la_SOURCES = \
	compiler.c \
	libmoxie.h \
	libmoxie-access.h \
	libmoxie-compiler.h \
	libmoxie-config.h \
	libmoxie-opcode.h
	
libmoxie_trace_la_SOURCES = \
	trace.c
	libmoxie.h \
	libmoxie-access.h \
	libmoxie-config.h \
	libmoxie-opcode.h

libmoxie_la_LIBADD = libmoxie-compiler.la libmoxie-trace.la
libmoxie_la_SOURCES = \
	access.c \
	moxie.c \
	profiler.c \
	libmoxie.h \
	libmoxie-access.h \
	libmoxie-config.h \
	libmoxie-opcode.h \
	profiler.h

