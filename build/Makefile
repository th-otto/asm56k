prefix = /usr/local
bindir = $(prefix)/bin

#-----------------------------------------------
#target file name
#-----------------------------------------------

TARGETNAME=asm56k

#-----------------------------------------------
#srcdir is a path to your top level "buildfiles" file
#-----------------------------------------------
srcdir=../src
opt_src=../src/opt/src

SRCS = \
	$(srcdir)/CodeUtils.c \
	$(srcdir)/ConvertFields.c \
	$(srcdir)/ErrorMessages.c \
	$(srcdir)/MacroProxy.c \
	$(srcdir)/OutputEmbededAsm.c \
	$(srcdir)/OutputLod.c \
	$(srcdir)/OutputP56.c \
	$(srcdir)/StringBuffer.c \
	$(srcdir)/SymbolTable.c \
	$(srcdir)/TokenStream.c \
	$(srcdir)/Value.c \
	$(srcdir)/main.c \
	$(srcdir)/codegen/GenArith.c \
	$(srcdir)/codegen/GenBitOps.c \
	$(srcdir)/codegen/GenControl.c \
	$(srcdir)/codegen/GenMisc.c \
	$(srcdir)/codegen/GenParMoves.c \
	\
	Parser.c \
	Lexer.c \
	\
	$(opt_src)/ag.c \
	$(opt_src)/opt_menu.c \
	$(opt_src)/opt_num.c \
	$(opt_src)/opt_proc.c \
	$(opt_src)/opt_reg.c \
	$(opt_src)/opt_regt.c \
	$(opt_src)/opt_util.c \
	$(empty)

#-----------------------------------------------
# compilers
#-----------------------------------------------

#EVAL_CROSS := case `uname -s` in *MiNT*) echo no;; *) echo yes; esac
#CROSS := $(shell $(EVAL_CROSS))

ifeq ($(CROSS),yes)

CROSSPREFIX=m68k-atari-mint-

NATIVECC = gcc

else

NATIVECC = $(CC)

endif


CC=$(CROSSPREFIX)gcc
CFLAGS=-O2 -Wall -Wstrict-prototypes -Wmissing-prototypes

LDFLAGS=-s
LD=$(CC)

LEX=flex
LEXFLAGS=-Pasm -Cem -8 

PYTHON = /usr/bin/env python

all: $(TARGETNAME)

OBJS = $(SRCS:.c=.o)

LIBS = -lm

CPPFLAGS = -I. -I$(srcdir) -I$(opt_src)

$(OBJS): Parser.h

Parser.h: Parser.c

Parser.c: $(srcdir)/Parser.y
	bison -o $@ --defines=Parser.h $<

Lexer.c: $(srcdir)/Lexer.l
	flex -Pasm -Cem -8 -o $@ $<

#------------------------------------------------------------------------------------
clean::
	@echo Cleaning...
	rm -f $(TARGETNAME) $(OBJS) Parser.c Parser.h Lexer.c

#------------------------------------------------------------------------------------
# Link ...
#------------------------------------------------------------------------------------

$(TARGETNAME): $(OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

#------------------------------------------------------------------------------------



test::
	$(PYTHON) ./test.py

install:: $(TARGETNAME)
	cp $(TARGETNAME) $(DESTDIR)$(bindir)

