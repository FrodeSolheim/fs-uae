YFLAGS=-d

LFLAGS=

AUTOMAKE_OPTIONS = foreign

noinst_LIBRARIES = libdebug.a

libdebug_a_SOURCES = asm.cc asm.h ppcdis.cc ppcdis.h ppcopc.cc ppcopc.h tracers.h \
debugparse.y debugtype.h lex.l lex.h parsehelper.c parsehelper.h debugger.cc debugger.h \
stdfuncs.cc stdfuncs.h debugparse.h x86opc.cc x86opc.h x86dis.cc x86dis.h

INCLUDES = -I ..
