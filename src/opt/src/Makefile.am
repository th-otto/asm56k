#AUTOMAKE_OPTIONS = no-dependencies

include_HEADERS  = opt.h

## Order is important here, opt_sf.h should be first
noinst_HEADERS   = opt_sf.h ag.h opt_p.h

## opt.c is a convenient amalgamation of all the other source files
BUILT_SOURCES = opt.c optcc.cc

lib_LIBRARIES    = libopt.a
libopt_a_SOURCES = opt_util.c opt_reg.c opt_regt.c opt_menu.c opt_proc.c ag.c opt_regcc.cc opt_num.c

EXTRA_DIST= makeopth.pl
CLEANFILES= opt.c optcc.cc

## The following is used only if --with-readline is invoked
## xrlo is list of object files in libreadline.a
## xtco is list of object files in libtermcap.a
libopt_a_LIBADD  = @xrlo@ @xtco@
libopt_a_DEPENDENCIES = @xrlo@ @xtco@

readline_PATH = @readline_PATH@
termcap_PATH =  @termcap_PATH@

@xrlo@: $(readline_PATH) 
	$(AR) x $(readline_PATH)

@xtco@: $(termcap_PATH)
	$(AR) x $(termcap_PATH)



####################
##
##      Make a single source file; this is useful for attaching
##      opt source to other packages without a lot of overhead
##

opt.c:  $(noinst_HEADERS) $(libopt_a_SOURCES)
	echo "#define OPT_PACKAGE \"@PACKAGE@\""  > opt.c
	echo "#define OPT_VERSION \"@VERSION@\"" >> opt.c
	echo "#include" \"opt.h\"            >> opt.c
	for f in $(noinst_HEADERS) $(libopt_a_SOURCES); do \
		cat $(srcdir)/$$f >> opt.c; done

optcc.cc: opt.c
	ln -f opt.c optcc.cc

###






