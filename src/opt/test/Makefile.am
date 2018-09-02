# $Id: Makefile.am,v 3.17 2002/09/10 00:00:08 simes Exp $

AUTOMAKE_OPTIONS = no-dependencies

CLEANFILES= birthday.opt birthday.opt~

TESTS = t_num checkopt checkoptcc
noinst_PROGRAMS = t_num testopt birthday hooktest iloop testmain testtypes tstring tnull manyopts tarray testcc testlong
EXTRA_DIST= checkopt checkoptcc

##I would have thought checkopt was a noinst_SCRIPT, but it doesn't
##even get included in the distribution if I do that
##noinst_SCRIPTS= checkopt

## Uncomment the appropriate line depending on whether you want to
## test libopt.a or opt.o (set this in configure script??)
##TESTLIB = ../src/opt.o
TESTLIB= ../src/libopt.a
LDADD = $(TESTLIB)

INCLUDES = -I$(top_srcdir)/src

DEPENDENCIES = $(TESTLIB)

## This remakes the library if it doesn't exist

RANLIB = @RANLIB@

../src/libopt.a:
	cd ../src; $(MAKE) libopt.a; $(RANLIB) libopt.a

../src/opt.o:
	cd ../src; $(MAKE) opt.o

../src/opt.c:
	cd ../src; $(MAKE) opt.c

##

testcc_SOURCES= testcc.cc

#testcc_LDADD= optcc.o
#optcc.o: ../src/opt.c
#	$(CXXCOMPILE) -c -o optcc.o ../src/opt.c




