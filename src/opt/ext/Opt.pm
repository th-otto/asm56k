# Opt.pm -- Perl Module for Options Parsing
# Copyright (C) 1998, James Theiler; email: jt@lanl.gov
# $Id: Opt.pm,v 1.3 2003/03/13 23:05:27 jt Exp $

=head1 NAME

Opt - parsing of command line options, and more...

=head1 SYNOPSIS

    use Opt qw($OPT);
    Opt::getoptions(["-x","Help string for x",\$x],
                    ["-yINT"],
                    ["--zvalue=STRING"], 
                    ["--mobile",\%StuckInsideOf]
                   );


and then the values associated with those options will be assigned
to variables   
C<$x>, C<$OPT{y}>, C<$OPT{zvalue}>, and C<$StuckInsideOf{mobile}>.

=head1 DESCRIPTION

The B<Opt> module is a library whose purpose is to simplify the task of
parsing command line arguments to set parameter values for a B<Perl>
routine.  B<Opt> also gets parameter values from environment variables, 
option files, and a builtin rudimentary interactive menu.
It is much like the
standard Getopt modules, but there are a few more bells and whistles.
It is mostly meant to be a clone of the similarly named B<opt> for C
programs (in fact, this version of F<Opt.pm> was probably distributed
with the larger C library).  

B<Opt> supports simultaneous use of traditional, bundled, and long (aka POSIX) 
options

=over 4

=item -x

turns on the 'x' feature of your code; in this traditional 
specification, the option specifier 'x' is only permitted to be
one character, usually alphabetic of either case, but Opt.pm also
permits other characters, such as the digits 0-9, so you can specify
'-2' to indicate that you want a two-sided statistical test.  Most
punctuation would not be permitted.

=item -x -y

turns on the 'x' and 'y' features

=item -xy

turns on both 'x' and 'y' features; putting both options together like
this is called "bundling".

=item -z 3.14159

specifies that the number associated with 'z' should have value 3.14159.

=item -z3.14159

does the same thing, even though there is no space between the 'z' and
'3.14159'; that space is optional.

=item --xflag

turns on the flag associated with "xflag"; note the two hyphens and 
that "xflag" is not constrained to be a single character.

=item --zvalue 3.14159
=item --zvalue=3.14159

are both permitted, but "--zvalue3.14159" is not allowed.

=back

as well as some extra bells and whistles specific to B<Opt>.

=over 4

=item @file.opt

specifies that options are read from a file named "file.opt".

=item %file.opt

tells the program to write all the current values of all the 
options to a file called file.opt.  If you want a "file.opt" template
as a starting point which you might then edit by hand, you can always
get one by typing C<program %file.opt .>; using a period ('.') as
an argument causes the program to exit gracefully.

B<Aside:>
Hmmm, this could be a problem for a program in which '.' might be a
reasonable argument, eg a program that was expecting a directory name.
Perhaps, we should instead use something like '---exit' instead?

=item --help

is a special flag which, if it is the first in a command line,
tells the program to write a help message and exit.  The message is
of a "standard" format (standard to 'opt' that is) which can be parsed
by other programs, such as 'tkopt' which instantly turns any opt-enabled
program to be GUI too.

=item --usage

is a special flag, like --help.  if it is the first in a command line,
tells the program to write a short usage message and exit.

=item --version

is a special flag, like --help.  if it is the first in a command line,
tells the program to write the version number and exit.

=item --menu

pops you into an interactive menu which allows you to see what parameters
are available, what they do, and what their current values are.  you can then
reset those parameters to what you want and run the program.

=back

=head1 Functions

=over 4

=item C<getoptions(['--var=TYPE','Help string',\$var], ['-x'], ... )>

In principle, C<getoptions> is the only function you need to know about
in order to use the B<Opt> package.  Each argument "registers" a single
option, which essentially sets up a table that associates each option with attributes specifying
what kind of option it is, and which variable in the B<Perl> script it
corresponds to.  After all the options are registered, the C<@ARGV> array
is parsed, and the variables are all set accordingly.  This is the
one-function-does-all invocation; you may prefer to call its individual
components: for instance, the example in the L<SYNOPSIS> is equivalent to:

    Opt::register('-x','Help string for x',\$x);
    Opt::register('-yINT');
    Opt::register('--zvalue=STRING');
    Opt::register('--longvar',\%LocalHashOfOptions);
    @ARGV=Opt::opt($0,@ARGV);

The longer form is a little closer to what the interface would look
like in the C version of B<opt>.  You may prefer this form
if, say, you want to perform argument processing on an array other than @ARGV.

=item register('--var=TYPE','Help string',\$var)

This function registers a single option of type TYPE, named 'var', and
associates it with the variable C<$var>.  Invocations of B<--var=value>
on the command line will lead to setting C<$var='value'> in the
B<Perl> script.  The help string is optional, as is the C<$var>
reference.  If the variable reference is not specified, then a
variable C<$OPT{var}> is created, and it is associated with the
option.  TYPE is one of: C<NUL>, C<INT>, C<UNSINT>, C<SHORT>, C<LONG>,
C<CHAR>, C<INTLEVEL>, C<FLOAT>, C<DOUBLE>, C<FLAG>, C<NEGFLAG>,
C<ABSFLAG>, C<ABSNEGFLAG>, C<STRING>, or C<UNDELIM>.

=item optreg(\$var,"TYPE",'v',"Help string")

This is another version of the C<register> function.  It is arguably
not as intuitive as C<register>, but it more closely matches the way
that options are registered in the C version of B<opt>.  This is not
the most convenient way to register a function, but it mimics the C
opt version.  This form only registers one-character option names.
Alternative forms include:

=over 4

=item optrega(\$var,"TYPE",'v',"var","Help string")

Provides two names for the variable C<$var>, a long name ("var") and a
short name ('v').  As an implementation issue, all other registration
functions, including the ones below as well as C<register> and even
C<getoptions>, all call C<optrega> to actually register the option.

=item optregc(\$var,"TYPE",'v')

only provides a short single-character name ('v') for the variable C<$var>.

=back

Each of these functions also has a form C<optreg_TYPE(\$var,'v',"Help string")>,
in which the TYPE is not a string argument but is part of the function name.

=item @argvnew=opt($0,@argv)

After all the options are registered, the function C<opt> does the
actual parsing of command line as given in the array C<@argv>.
Here C<$0> is the name of the program.  Note that C<opt> does
"nondestructive" argument processing; so the argument C<@argv> is unaltered
by the call to C<opt>.  The result C<@argvnew> is the list of arguments that
are unprocessed by C<opt>.  In typical usage, you would write
C<@ARGV=Opt::opt($0,@ARGV);>

=head2 Hooks

Hooks are functions which are written by the B<Perl>
application programmer (but not by the B<Opt> developer), which B<Opt> calls
at certain points in its processing.  Among them are:

=over 4

=item C<useUsageLines("Usage: \%s [options]")>

specifies the first line(s) of the usage message.  The '\%s' will
be expanded out as the program name.  Multiple lines can be provided
as separate arguments.   

=item C<useMainFcn(\&MyMain);>

specifies that the function C<&MyMain> will be run whenever the "=" is
invoked on the opt menu.

=item C<useEnvString('ENVAR');>

specifies that the environment variable can be used to specify default
options.  eg, if C<ENVAR='-x+ -y-'>, then the default value for
'x' will be TRUE, and for 'y' will be FALSE.

=item C<useHash(\%OPTION);>

specifies that options registered without an explicitly associated
reference will be associated with the hash %OPTION.  That is,

    %OPTION=();
    useHash(\%OPTION);
    Opt::register("-x");

will associate the option B<-x> with the variable C<$OPTION{x}>.
By default, the package does an equivalent of C<useHash(\%OPT)>,
unless that is changed by this function or by the C<useScalarPrefix>
function.

=item C<useScalarPrefix('opt_');>

specifies that options registered without an explicitly associated
reference will be associated with the scalar string C<$opt_>I<xxx>,
where I<xxx> is the name of the option.

=item C<noScalarPrefix('opt_');>

disables scalar prefix for options.

=item C<useInitFile('/etc/progrc')>

still not implemented; equivalent to B<@/etc/progrc> as the first 
argument in the command line.  Useful for site-wide configurations.

=back

=cut

package Option::Base;

=pod

=head1 IMPLEMENTATION DETAILS

=head2 The Option Classes

An I<Option> is an object which contains information about an individual
option (name, help string, variable they refer to, and type), as well
as methods for manipulating it, eg reading its value from a
string, writing its value to strings appropriate for menus,
optfiles, etc.

Option::Base is the base option class; it is an actual working class
(not an "abstract class" in the sense of C++), and is actually used
for string options, but most of the other classes are derived from it.

The derived classes are for different kinds of options; currently
these options are:

   Option::Base       NUL? STRING UNDELIM CSTRING UNDELIMC
   Option::Numeric    FLOAT DOUBLE
   Option::Int        INT SHORT LONG
   Option::UInt       UINT USHORT ULONG
   Option::IntLevel   INTLEVEL
   Option::Flag       FLAG NEGFLAG ABSFLAG ABSNEGFLAG

Each object contains (among other things) a reference to a scalar
variable.  Getting and setting the value of this variable is the
whole point of the B<Opt> package.  Since the I<"variable
associated with the given class object"> is a cumbersome phrase, I will
refer to that variable as the I<optvar>

=head2 Methods

The methods defined in these classes are:

=over 4

=item default()

returns a "default value" for the class; it is the empty string
for STRING classes, and zero for numeric classes.

=item getval()

returns the value of the variable
associated with the given class object; ie, the value of the I<optvar>.

=item setval(var)

sets the I<optvar> to the value of the variable in the argument.
(Should I call that the I<argvar>?)  Generally, C<setval()> is
implemented with a call to C<fixval()>, so that it sets the I<optvar>
to a valid value.

=item fixval(var)

does not affect the I<optvar> at all.  It converts the argument value 
to a value that is consistent with the argument type, and returns
the consistent value. eg, if the option is a UINT (unsigned integer),
then with an argument of -33.5, B<fixval> will return 33.

=item valOf(string)

does not affect the I<optvar>.  It parses the input argument, usually
thought of as a string (but this is Perl, so it can in practice be
just about anything), and returns a value 

For many data types, C<valOf()> is an identity function, returning the
same value that was provided as an argument.  But for some types, it
does a little massaging.  For example, if the option is a FLAG type,
then C<valOf("+")> will return a 1; if the option is a NEGFLAG,
then C<valOf("+")> will return a 0.

If the function C<valOf()> is called without an argument, then it sets 
I<optvar> as if the option had been invoked without an argument.  
For example, if you want B<-f>
to turn on a flag C<$f>, then C<setvalOf()> will toggle the value 
of the flag variable C<$f>.

=item setvalOf(string)

is equivalent to C<setval(valOf(string))>;
that is, it sets the I<optvar> according to the argument string.

=item requiresArg()

returns 0 or 1 according to whether the option requires an argument;
eg, an integer option requires an argument because you use B<-n15> or
B<-n 15> to set the value to 15, the lone string B<-n> makes no sense.
For a flag option, by contrast, a lone B<-f> on the command line is all
that is needed to toggle the flag.

=item toString()

returns a string representation of the value of I<optvar> that is
appropriate for writing to a file (that can then be read by programs
using B<Opt>).  For example, if the option is a FLAG type, and it
has a value of 1, then C<toString()> will return "+".

=item toMenu()

is much like toString(), except that it returns a stringified I<optvar> 
that is appropriate for the builtin menu.

=item toMenuLine()

writes out the whole line that appears in the interactive menu; it is
standardized formatting with a call to C<toMenu()> to get the value of
the I<optvar>.

=item toFileLine()

is similar to C<toMenuLine()>, except that it writes a line appropriate
for the full line of an .opt file.  Thus it includes the name of the option,
its C<toString()>ified value, and a commented-out help string.

=back

Thanks to the magic of inheritance, we do not have to define every one
of these functions for every data type.  The table below shows how
for which classes the different functions need to be defined.  An C<x>
indicates an implementation of that function for that class, while
C<...> indicates that the function is inherited.

                       Inheritance Table
                       -----------------
  Function    Class: B   N   I   U   L   F
  default            x   x................
  getval             x....................
  setval             x....................
  fixval             x   x   x   x....   x
  valOf              x............   x   x 
  setvalOf           x....................
  requiresArg        x............   x....
  toString           x............   x   x
  toMenu             x............   x   x
  toMenuLine         x....................
  toFileLine         x....................

The classes (which are columns in this table) are: B=Base, N=Numeric,
I=Integer, U=UnsignedInt, L=IntLevel, and F=Flag.  The inheritance hierarchy is essentially linear, with each class being derived from the class to its
left in the above table.  Conceptually, the hierarchy is shown below,
except that there is no S=String class (we just use the B=Base class,
and check if C<$$self{type} =~ /STRING|UNDELIM/> for the few occasions
where strings behave differently from the base.

     +- S
     |
  B -+- N --- I --- U --- L --- F               

=cut

  ## Opt, on the other hand, is not an object class (yet), but a
  ## package. It "has a" list of Option objects, actually of object
  ## refs.  As well as the list, it also has two hashes, various
  ## global (well, global to Opt) variables indicating current mode:
  ## eg, Is menu being read, or is info really from command line?

sub new {
    my $class = shift;
    $class = ref($class) || $class; # see Camel Book, p295
    my $self = {};		    # ref to empty hash
    bless $self,$class;
    $self->_initialize(@_);
    return $self;
}
sub _initialize {
    my $self = shift;
    $$self{value}    = shift or warn "_initialize: value not specified";
    $$self{type}     = shift or warn "_initialize: type not specified";
    $$self{name}     = shift || "";
    $$self{longname} = shift || "";
    $$self{descript} = shift || "";

    if (not defined ${$$self{value}}) {
        $self->setval($self->default);
    }
}
sub default {
    my $self = shift;
    return "";
}
## getval returns the actual value of the optvar
sub getval {
    my $self = shift;
    return ${$$self{value}};
}
## fixval doesn't affect the value of the optvar, it simply makes sure
## that the input argument is consistent with the type
## In the base class, it is a simple identity.
sub fixval {
    my $self = shift;
    my $val = shift;
    return $val;
}
## setval sets the value of the optvar to the value of the argument,
## with that value possibly "fixed" to make it consistent with the type.
## readval sets the value according to the string that the user specifies 
## on the command line
sub setval {
    my $self = shift;
    my $val = shift;
    $val = $self->fixval($val);
    ${$$self{value}} = $val;
}
## valOf translates the string argument into a value; it doesn't
## affect the value of the optvar.
## In the base class, it too (like fixval) is a simple identity.
sub valOf {
    my $self = shift;
    my $string = shift;
    return $string;
}
sub setvalOf {
    my $self = shift;
    my $string = shift;
    $self->setval($self->valOf($string));
}

sub requiresArg {
    my $self = shift;
    ## except for some derived types (namely FLAG's and INTLEVEL's)
    ## most options require an arg, eg you can't say '-x' you know to
    ## expect some value afterward: eg, '-x27' or '-x 27'. 
    return 1;
}
## toString converts the optvar into a string which can be used when
## writing the value of the optvar out to an .opt file or to a menu.
sub toString {
    my $self = shift;
    my $string = $self->getval;
    if ($$self{type} =~ /STRING|UNDELIM/) {
	## if its a STRING type, put quotes around it
	## Go ahead and escape existing quotes too 
	## eg, [hello "world"] --> ["hello \"world\""]
	$string =~ s/\"/\\\"/g;
	$string='"' . $string . '"';
    }
    $string;
}
sub toFileLine {
    my $self = shift;
    my $line="";
    if ($$self{name} ne "") {
	$line .= "-$$self{name}";
	$line .= " " if $self->requiresArg;
    } elsif ($$self{longname} ne "") {
	$line .= "   --$$self{longname}=";
    }
    $line .= $self->toString;
    $line .= (" "x(30-length($line))) . "; ";
    if (my $d=$$self{descript}) {
	my @dsc = split("\n",$d);
	$line .= shift @dsc;
	foreach $d (@dsc) {
	    $line .= "\n" . (" " x 30) . "; ".$d;
	}
    }
    $line;
}


sub toMenu {
    my $self = shift;
    return $self->toString();
}

sub toMenuLine {
    my $self = shift;
    my $line = "";
    if ($$self{name} ne "") {
	$line .= "$$self{name} ";
	$line .= $$self{descript} if $$self{descript};
    } elsif ($$self{longname} ne "") {
	$line .= "   --$$self{longname}";
    }
    $line .= " "x(30-length($line));
    $line .= $self->toMenu;
}
=pod

A few auxiliary functions are also defined which do not affect the
I<optvar>, but deal with formatting of the option name.

=over 4

=item hasname()

returns 1 if the option has a short (one-character) name that can be
invoked from the command line preceeded by a single dash (eg, B<-x>).

=item haslongname()

returns 1 if the option has a long (typically multi-character) name
that can be invoked from the command line preceeded by a 
double dash (eg, B<--xcaliber>).

=item thename()

returns the short name if it exists; if there is no short name, then
this function returns the long name.  if there is neither a short nor
a long name, the function returns 'undef'.

=back

=cut    

sub hasname {
    my $self = shift;
    return ($$self{name} ne "" ? 1 : 0);
}
sub haslongname {
    my $self = shift;
    return ($$self{longname} ne "" ? 1 : 0);
}
sub thename {
    my $self = shift;
    return $$self{name}     if $$self{name};
    return $$self{longname} if $$self{longname};
    warn "Option [$self] has no name\n";
    return undef;
}

## Purely for debugging....
sub print {
    my $self = shift;
    print "name=",$self->thename(),", ";
    print "type=",$$self{type},", ";
    print "value=",$self->getval(),".";
    print "\n";
}
#######################################################################

package Option::Numeric;
@ISA = qw( Option::Base );
sub default {
    my $self = shift;
    return 0;
}

sub fixval {
    my $self = shift;
    my $val = shift;
    $val = 0 unless
	## Note: this regexp lifted from Greg Ward's Getopt::Tabular
	$val =~ /[+-]?(\d+(\.\d*)?|\.\d+)([Ee][+-]?\d+)?/;
    $val;
}

package Option::Int;
@ISA = qw( Option::Numeric );
sub fixval {
    my $self = shift;
    my $val = shift;
    $val = Option::Numeric::fixval($self,$val);	
    $val = int($val);
}
package Option::UInt;
@ISA = qw( Option::Int );
sub fixval {
    my $self = shift;
    my $val = shift;
    $val = Option::Int::fixval($self,$val);
    $val = -$val if $val < 0;
    $val;
}

package Option::IntLevel;
@ISA = qw( Option::UInt );

sub requiresArg {
    my $self = shift;
    return 0;
}
    
sub toString {
    my $self = shift;
    my $val = $self->getval;	
    ## may be safer: $val= $self->fixval($val),
    ## but that shouldn't be necessary; the value
    ## was set by going "through" fixval()
    
    ## IntLevel's have a strange toString(), if the value is 4, for
    ## example, then '-v -v -v -v' should set it to four, but that is
    ## four separate strings, and besides if the value starts out nonzero,
    ## this only increments by four.  We can fix the latter problem with
    ## '-v- -v -v -v -v' and we can fix the separate strings problem with
    ## bundling '-v-vvvv'.  Note that this means we can't register an
    ## INTLEVEL type with only a long name!
    my $string;
    if ($self->hasname) {
	$string = "-" . ($$self{name} x $val);
    } else {
	die "Cant have INTLEVEL without a short name";
    }
    $string;
}
sub toMenu {
    my $self = shift;
    my $string = $self->getval;

    $string = 0 unless $string =~ /^\d+$/; # non-numeric gets assigned zero
    if ($string == 0) {
	$string="OFF";
    } elsif ($string == 1) {
	$string="ON";
    } elsif ($string > 1) {
	$string="ON:".$string;
    }
    $string;
}
sub valOf {
    my $self = shift;
    my $val = shift || undef;

    if (defined $val) {
	$val =~ tr/+-/10/;
    } else {
	$val = 1+$self->getval;
    }
    $val;
}

package Option::Flag;
@ISA = qw( Option::IntLevel );

sub fixval {
    my $self = shift;
    my $val = shift || 0;
    $val = ($val ? 1 : 0);
}
sub valOf {
    my $self = shift;
    my $val = shift;

    if (defined $val) {
	## translate + to 1, - to 0;
	$val =~ tr/+-/10/;
	## make sure val is either 1 or 0
	$val = ($val ? 1 : 0);
	## if it's a NEGFLAG, then invert the value
	$val = ($val ? 0 : 1) if ($$self{type} =~ /NEG/);
    } 
    elsif ($self->{type} =~ /ABS/) {
	$val = 1;
	$val = ($val ? 0 : 1) if ($$self{type} =~ /NEG/);
    } 
    else {
	$val = ($val ? 0 : 1);
    }
    $val;
}

sub toString {
    my $self = shift;
    my $string = $self->getval;

    ## first get a boolean value
    $string = ($string ? 1 : 0);
    $string = ($string ? 0 : 1) if ($$self{type} =~ /NEG/);
    ## convert it to +/-
    $string = ($string ? "+" : "-");
}
sub toMenu {
    my $self = shift;
    my $string = $self->toString;
    $string = ($string eq "+" ? "TRUE" : "FALSE");
}
############################################################################

package Opt;

############################################################################

($VERSION) = '$Revision: 1.3 $ ' =~ /^\S+\s+(\S+)/;

use Exporter ();
@ISA=qw(Exporter);
@EXPORT_OK=qw(%OPT getoptions register optreg optrega);

my $DEBUG=0;

sub debug { $DEBUG=1; }

## Globals
## Far more than we need in perl, but these match
## what is available in the C code)
@optTypes = qw(NUL
	       INT        SHORT      LONG       CHAR
	       UINT       USHORT     ULONG      UCHAR
	       FLOAT      DOUBLE
               INTLEVEL       
	       FLAG       NEGFLAG    ABSFLAG    ABSNEGFLAG
               STRING     UNDELIM
	       CSTRING    UNDELIMC);

## 2. Global Variables

my @optList=();    ## optList is an array of Option's, each Option is an object
                   ## that is defined in the Option.pm package.  

my %optHash=();    ## optHash is that same set of Option objects, 
                   ## but hashed on the name.

my @optUndelim=(); ## list of undelimited options

%OPT=();         ## OPT is a hash of ${$opt->{value}}'s
                 ## This is exported by default, and is meant to be
                 ## a global variable in the calling perl script
$ROPT=\%OPT;     ## ROPT is a ref to OPT; We won't actually set $OPT{..}
                 ## Instead, we'll set via $ROPT, so that we can alter
                 ## the hash that we use.


%optParm=(
    ## These are parameters that OPT uses
    program      => $0,      ## Name of the program that invoked opt
    env          => undef,   ## Name of environment variable from which 
                             ## option defaults are defined
    main         => undef,   ## Pointer to fcn which is run as "main" 
                             ## whenever "=" specified on menu
    scalarprefix => undef,   ## if nonempty, eg if equal to "opt_", then
		             ## let $opt_xxx be variable associated with
		             ## option named xxx
    mode         => undef,   ## eg, MenuMode, CommandLineMode, FileMode
                             ## behavior varies in these different modes
    menumode     => 0,       ## menumode: specifies whether opt is currently 
                             ## taking options from menu input
    usageline    => "Usage: %s [options]",
                             ## First line in usage message
			     ## The %s expands to $optParm{program}
    progversion  => "",      ## Version of program    
    defaultsWithHelp => 0,
    );

## These functions are called to set the OPT parameters:

sub useUsageLines {
    $optParm{usageline}= join("\n",@_);
}
sub useVersion {
    $optParm{progversion} = shift;
 }
sub useMainFcn {
    $optParm{main} = shift;
}
sub useEnvString  {
    $optParm{env} = shift;
    print STDERR "Env=$optParm{env}\n" if $DEBUG;
}
sub useScalarPrefix {
    $optParm{scalarprefix} = shift || "opt_";
}
sub noScalarPrefix {
    $optParm{scalarprefix} = undef;
}
sub useHash {
    my $hashref = shift;
    print STDERR "ROPT=$ROPT, hashref=$hashref\n" if $DEBUG;
    ## Maybe should copy current key/val pairs in OPT to new OPT
    ## ...or maybe not!
    my $isHash = "string $hashref";
    die "useHash: $hashref is not a hash\n"
	unless $isHash =~ /HASH/;
    $ROPT = $hashref;
    print STDERR "ROPT=$ROPT, hashref=$hashref\n" if $DEBUG;
}
    
sub useDefaultsWithHelp {
    $optParms{defaultsWithHelp} = 1;
}

my $pkg = (caller)[0];         ## This is needed if we are going to make
                               ## all the 'opt_xxx' variables global to
                               ## the calling package (usually 'main')


#####################################
## Functions for registering options:
##
## The optregXXX functions follow the style of registering options in the
## C code, but are probably not the most convenient way to register them
## for a Perl application.
##
## Most of the various ways of registering options ultimately call optrega() 
## It takes specific arguments, defines a new Option, and appends it to the 
## list @optList.  The %optHash is not updated until one of the getopt fcn's
## is called.

sub optrega {
    my ($nptr,$type,$name,$longname,$descript)=@_;
    warn "optrega: not enough arguments" if scalar(@_) < 5;
    my $opt;
    warn "new type=$type" if $DEBUG;
    warn "new name=$name" if $DEBUG;
    warn "new longname=$longname" if $DEBUG;
    if ($type =~ /FLAG/) {
	$opt = Option::Flag->new(@_);
    } elsif ($type =~ /DOUBLE|FLOAT/) {
	$opt = Option::Numeric->new(@_);
    } elsif ($type =~ /INTLEVEL/) {
	$opt = Option::IntLevel->new(@_);
    } elsif ($type =~ /^(INT|SHORT|LONG)$/) {
	$opt = Option::Int->new(@_);
    } elsif ($type =~ /^U(INT|SHORT|LONG)$/) {
	$opt = Option::UInt->new(@_);
    } else {
	$opt = Option::Base->new(@_);
    }

    ## each $opt goes into the list
    push @optList,$opt;

    $opt;  ## if it must return something, might as well be the new $opt.
}    
## short form of optrega which doesn't do longname's
sub optreg {
    my ($nptr,$type,$name,$descript)=@_;
    my $longname="";
    &optrega($nptr,$type,$name,$longname,$descript);
}

## define all the subroutines of the form
## optreg_INT, optreg_STRING, etc.
foreach $type (@optTypes) {
    eval "sub optrega_$type { ".
	"my (\$nptr,\$name,\$longname,\$descript)=\@_; ".
	    "\$type=\"$type\"; ".
		"\&optrega(\$nptr,\$type,\$name,\$longname,\$descript); ".
		    "}";
    eval "sub optreg_$type { ".
	"my (\$nptr,\$name,\$descript)=\@_; ".
	    "\$longname=\"\"; \$type=\"$type\"; ".
		"\&optrega(\$nptr,\$type,\$name,\$longname,\$descript); ".
		    "}";
    ## Having defined them, now export them!
    push @EXPORT_OK,"optreg_$type","optrega_$type";
}

sub register {
    ## Usage:
    ## There are a number of ways that an option can be registered; the
    ## easiest is 
    ##     Opt::register("-x");
    ## This will make 'x' a FLAG option; if '-x' is set on the command line
    ## then the variable $Opt::OPT{x} will be set to 1, otherwise it is zero.
    ## If you want 'x' to be another kind of option, eg INT or FLOAT or STRING,
    ## then you can use, for example,
    ##     Opt::register("-xSTRING");
    ## Long (aka POSIX) options are also permitted, these are registered
    ## with a call like
    ##     Opt::register("--xvalue");
    ## or  Opt::register("--xvalue=INT");
    
    ## A second and third argument are also accepted by Opt::register().
    ## You can assign a help string, eg
    ##     Opt::register("-x","Use method x")
    ## or a direct reference to the variable
    ##     Opt::register("-x",\$x);
    ## or both
    ##     Opt::register("-x","Use method x",\$x);

    my ($string,$help,$hashref) = @_;

    print STDERR "Registering $string\n" if $DEBUG;

    ## if called as Opt::register("-x",\$x);
    if (defined($help) and (ref($help) eq "HASH" or ref($help) eq "SCALAR")) {
	my $tmp=$hashref;
	$hashref=$help;
	$help=$tmp;
    }
    print STDERR "HashRef=",$hashref,", Ref=",ref($hashref),"\n" if $DEBUG;
    unless (ref($hashref)) {
	## should check flag for variable prefix; if it exists
	## then don't use %OPT hash
	unless ($optParm{scalarprefix}) {
	    $hashref = $ROPT;
	}
    }
    
    ## For now string must begin with either '-' or '--'
    ## May consider being more permissive later, but for now
    ## I'm inclined to enforce this condition so that the applications
    ## programmer is clear about whether is a one or two dasher.
    unless ($string =~ /^-/) {
	die "argument to Opt::register [$string] should begin with [-],";
    }
    
    if ($string =~ /^-(\w)$/) {
	print STDERR "case 1: '-x' [$string]\n" if $DEBUG;
	$name = $1;
	$longname = "";
	$type = "FLAG";
    } elsif ($string =~ /^-(\w)\s*([a-zA-Z]+)/) {
	print STDERR "case 2: '-xTYPE' [$string]\n" if $DEBUG;
	$name = $1;
	$longname = "";
	$type = $2;
    } elsif ($string =~ /^--(\w[\w-]+)$/) { # permitting '-' in name, dangerous?
	print STDERR "case 3: '--xlabel' [$string]\n" if $DEBUG;
	$name = "";
	$longname = $1;
	$type = "FLAG";
    } elsif ($string =~ /^--(\w[\w-]+)\s*=\s*([a-zA-Z]+)/) {
	print STDERR "case 4: '--xlabel=TYPE' [$string]\n" if $DEBUG;
	$name = "";
	$longname = $1;
	$type = $2;
    } else {
	warn "Invalid option specification: [$string]\n";
	return;
    }
    ## also permit type = "int" or "Int" as well as "INT"
    $type = uc $type;

    ## for consistency with Getopt::Long
    $type = "INT"    if $type eq "i";
    $type = "FLOAT"  if $type eq "f";
    $type = "STRING" if $type eq "s";

    $help = ($longname || $name) unless $help;

    if ($optParm{scalarprefix} and not $hashref) {

	## if set, this identifies options of type $opt_xxx
	## since they need to be "global", they are prepended with "pkg::" 
        ## where pkg is the name of the calling package (usually main)

	## Note, we might check scalarprefix for trailing underscore.
	## If you set Opt::useScalarPrefix("opt_"), then it will make
	## the name $opt__xxx with two underscores!

	print STDERR "scalar prefix = $pkg"."::".$optParm{scalarprefix}."\n" 
	    if $DEBUG;
	my $varname = $name || $longname;
	print STDERR "varname=$varname" if $DEBUG;
	$varname =~ s/\W/_/g;	# make into a valid variable name
	print STDERR "=$varname" if $DEBUG;
	$varname = $pkg."::".$optParm{scalarprefix}."_".$varname;
	print STDERR "=$varname\n" if $DEBUG;
	print STDERR "\$vref = \\\$".$varname,"\n" if $DEBUG;
	eval "\$vref = \\\$".$varname;
	if ($DEBUG) {
	    eval "\$".$varname."=17";
	    print STDERR "vref=",$$vref,"\n";
	}
    } else {
	my $label = $name || $longname;
	# But what if $hashref is an object which is really a hash?
	#$vref = (ref($hashref) eq "HASH" ? \$hashref->{$label} : $hashref);
	$vref = (ref($hashref) eq "SCALAR" ? $hashref : \$hashref->{$label});
    }    

    &optrega($vref,$type,$name,$longname,$help);

}

## Opt::getoptions is a single function call that does it all; it
## registers options according to its argument list (each argument
## is an anonymous array which is a list of arguments to be sent
## to "Opt::register); finally, Opt::opt is called.
sub getoptions_usage {
    my $msg = shift;
    print STDERR "getoptions error: $msg\n";
    print STDERR "Usage: arguments are anon arrays\n";
    print STDERR "Opt::getoptions(['-x'],          ## traditional flag\n";
    print STDERR "                ['-ySTRING'],    ## traditional parameter\n";
    print STDERR "                ['--zvalue=INT'] ## POSIX parameter\n";
    print STDERR "                ['--etc','Help for etc',\\\$etc]\n";
    die "\n";
    
}
sub getoptions {
    my $argref;
    while ($argref = shift) {
	getoptions_usage "Invalid option specification" 
	    if (ref($argref) ne "ARRAY");
	register(@$argref);
    }
    @ARGV = opt($0,@ARGV);
}
	
sub printVersion {
    print $optParm{program}," ",$optParm{progversion},"\n";
}
sub printUsage {
    print join("\n",&usageLines()),"\n";
}
sub printHelp {
    print join("\n",&helpLines()),"\n";
}
sub usageLines {
    my @usagelines=();
    push 
	@usagelines,
	sprintf($optParm{usageline},$optParm{program}),
	"To get a brief usage message, type: $optParm{program} --usage",
	"To get a longer help message, type: $optParm{program} --help",
	"To invoke the menu, type:           $optParm{program} --menu";
    return @usagelines;
}
    
sub helpLines {
    my @helplines= &usageLines();
    push @helplines, "The options are:";

    foreach $opt (@optList) {
	my $line=" ";
	if ($$opt{name} ne "") {
	    $line .= "-$$opt{name}";
	    $line .= ($$opt{longname}) ? ", " : "  ";
	} else {
	    $line .= "    ";
	}
	if ($$opt{longname} ne "") {
	    $line .= "--$$opt{longname} ";
	}
	$line .= " " x (20-length($line));
	$line .= "<" . $$opt{type} . ">";
	$line .= " " x (35-length($line));
	my $lln = length($line);
	my @dsc = split("\n",$$opt{descript});
	$line .= shift @dsc;
	foreach $d (@dsc) {
	    $line .= "\n" . (" " x $lln) . $d;
	}
	if ($optParms{defaultsWithHelp}) {
	    $line .= "\n" . (" " x $lln) . "Default=" . $opt->toMenu;
	}
	push @helplines, $line;
    }
    ## push @helplines, " --"; ## Ugly...maybe useful, but ugly.
    @helplines;
}

sub printToFile {
    my $file = shift;
    if ($file eq '%') {     
	my $prog = $optParm{program};
	$prog =~ s,\.[^/]*$,,;	# remove trailing extensions
	$file = "$prog.opt"; 
    }

    print STDERR ">>>> output to file: $file\n" if $DEBUG;
    open(OUT,">$file") || 
	warn "Cant open file [$file] for writing options: $!\n";
    foreach $opt (@optList) {
	print OUT $opt->toFileLine(),"\n";
    }
    close OUT;
}
sub startMenu {
    $optParm{menumode} = ($optParm{menumode} ? 0 : 1);
    print &menuString if $optParm{menumode};
    while ($optParm{menumode}) {
	my @argv = &readFromMenu();
	opt_process(@argv);
    }
}
sub menuString {
    my $string="";
    foreach $opt (@optList) {
	$string .= $opt->toMenuLine . "\n";
    }
    $string;
}

$readFromMenuYet=0;
sub readFromMenu {
    print "(Type ? for help)\n" if not $readFromMenuYet;
    ++$readFromMenuYet;
    print "-> ";
    my $line = <STDIN>;		# better to read from /dev/tty ??
    chomp($line);
    if ($line) {
	$line = '-' . $line if $line =~ /^\w/;
	@argv = &line2argv($line);
    } else {
	print &menuString;
    }
}
    
sub readFromFile {
    my $file = shift;
    my $origfile = $file;
    if ($file eq '@') {     
	my $prog = $optParm{program};
	$prog =~ s,\.[^/]*$,,;	# remove trailing extensions
	$file = "$prog.opt"; 
    }
    print STDERR ">>>> atsign $file\n" if $DEBUG;
    open(IN,$file) || die "Cant open file [$file] for reading options: ".
	( ($origfile eq '@') ? "(note \@\@ -> \@$file): " : "") . "$!\n";

    while (<IN>) {
	chomp;
	## ignore lines which begin with a `;'
	next if /^\s*;/;
	## Strip leading whitespace
	s/\s*//;
	## now call opt recursively, using the line itself
	## as the new argv;
	&opt_process(&line2argv($_));
    }
    close IN;
}

## We want to be able to use the `;' as a comment character so
## 'blah blah ;comment' will return @argv=(blah,blah), but...
## We want to be able to pass arguments with semicolons in them, eg
## 'blah "blah ;comment"' should give @argv=(blah,blah ;comment)
## Although we split the string into words based on whitespace,
## 'blah blah;comment etc' should return @argv=(blah,blah)
## Trickier: 'blah "bl ah";comment'
## The following seems to achieve all of these effects.
sub line2argv {
    ## used by both &readFromFile, and &readFromMenu;
    use Text::ParseWords;
    my $line = shift;
    ## First break the line up into space-separated words
    ## quotewords() recognizes quotation marks and backslashes
    ## But at this point no attention is paid to semicolons
    ## The second argument, 1, indicates that the works keep those
    ## quotation marks or backslashes.
    my @argvtmp = quotewords("[ \t\n\r\f]+",1,$line);
    my @argv = ();
    foreach $a (@argvtmp) {
	## Now, on a word by word basis, we split each word 
	## based on semicolons.  
	## Note, at this stage, second arg, 0, indicates that 
	## we strip the quotes and backslashes.
	my @aargv = quotewords(";",0,$a);
	## Usually, with no unescaped semicolons, this split 
	## produces only one word, and it is pushed onto @argv.
	my $aa = shift @aargv;
	push @argv,$aa if $aa;
	## If there is more than one word, when split this way, 
	## then that indicates a comment-semicolon has been reached.
	## We already push'd the first word, we toss the rest, and
	## in fact, toss the whole rest of the line.
	last if @aargv;
    }
    return @argv;
}
    
## opt reads a command-line argument list (passed as the argument)
## parses the options, sets variables, etc.; it returns that unprocessed
## portion of the argument list
sub opt {

    print STDERR "EXPORT=",join(":",@EXPORT),"\n" if $DEBUG;

    $optParm{program} = shift;
    $optParm{program} =~ s,.*/,,;

    if (defined($_[0]) and $_[0] eq "--help") {
	&printHelp();
	exit;
    }
    if (defined($_[0]) and $_[0] eq "--usage") {
	&printUsage();
	exit;
    }
    if (defined($_[0]) and $_[0] eq "--version") {
	&printVersion();
	exit;
    }
    ## Initialize
    %optHash=();
    @optUndelim=();		# is there any occasion where this
				# needs to be reset??
    foreach $opt (@optList) {
        ## put both names into the hash 
        $optHash{$opt->{name}}     = $opt if $opt->hasname;
        $optHash{$opt->{longname}} = $opt if $opt->haslongname;

	## put $opt in list of undelimited options, if that's what it is
	push @optUndelim,$opt if $opt->{type} =~ /UNDELIM/;
    }

    ## This is the place where you'd check for a .progrc file

    ## Check to see if there is an environment variable
    if (defined $optParm{env}) {
	if (defined $ENV{$optParm{env}}) {
	    &opt_process(line2argv($ENV{$optParm{env}}));
	}
    }


    ## Now process the arguments on the command line	    

    &opt_process(@_);
}
sub opt_process {
    my @argv = @_;		# this copies the arg list; thus there is
				# no "destruction" of @ARGV
    my @retargv = ();
    while ($_=shift @argv) {

	## a plain '--' ends argument processing
	last if /^--$/;

	if (/^=/ and $optParm{menumode}) {
	    my @arg = @argv;
	    if ($optParm{main}) {
		&{$optParm{main}}(@arg);
		1 while shift @argv;
	    } else {
		## if a 'main' hasn't been defined, then quit the menu 
		## note: &startMenu() when menumode=1 quit's the menu
		&startMenu();
	    }
	    next;
	}
	if (/^\$$/ || /^--menu$/) {
	    &startMenu();
	    next;
	}
	if (/^\.$/) {
	    exit;
	}
	if (/^@/) {
	    my ($file) = /^@(.*)/;
	    &readFromFile($file);
	    next;
	}

	if (/^%/) {
	    my ($file) = /^%(.*)/;
	    &printToFile($file);
	    next;
	}
	
	if (/^\?/) {
	    my ($name) = /^\?-*(.*)/;
	    if ($name) {
		my $help = $name.": ".$optHash{$name}->{descript}."\n";
		$help =~ s/\n\n/\n/g;
		$help =~ s/\n/\n     /g;
		$help =~    s/\n     $/\n/;
		print $help;
	    } else {
		my $prog = $optParm{program};
		$prog =~ s,\.[^/]*$,,;	# remove trailing extensions
		my $file = "$prog.opt"; 
		print <<EOPT;
        ?<varName>          Help for specific variable
        --varName=Value     Set value of variable
        @<filename>         Get opts from file
        @@                  Get opts from default file [$file]
        %<filename>         Put opts in file 
        %%                  Put opts in default file [$file]
        .                   Quit                
EOPT
    
            }
	}

	## all options start with a '-'
	## anything that doesn't is added to the returned argv
	## this is arguably too "permissive", as it allows 
	## options to be parsed after the main arguments, eg
	## 'cat file -n' works the same way as 'cat -n file'.
	## if you don't like opt to be so permissive, then
	## instead do something like:
	## push @retargv,$_ while (shift @argv);
	unless(/^-/) {
	    ## If there are any UNDELIM opts waiting to be filled,
	    ## then fill them here.  Note we have ruled out the
	    ## use of undelim strings that start with '-'
	    if (@optUndelim) {
		$opt = shift @optUndelim;
	    } else {
		push @retargv,$_;
		next;
	    }
	}
	
        ##                             name, rest
	##                            ------------
	## Cases here 1/ -xval     ==> x,    val
        ##            2/ --xval    ==> xval, undef
	##            3/ --xval=   ==> xval, ""
	##            4/ --xval=8  ==> xval, 8

	## Strip the leading '-';
	s/^-//;

	my ($name,$rest) = /^(.)(.*)$/;   ## works for Case 1

	if ($name eq "-") {
	    ## ie, if Case 2, 3, or 4

	    ## this works for Case 2
	    $name = $rest;
	    $rest = undef;
	}
	my $eqsign=0;                     ## is there an '=' in the string?
	if ($name =~ /=/) {
	    ## ie, if Case 3 or 4
	    $eqsign=1;
	    ## Maybe we should be consistent with our pattern 
	    ## in Opt::register, ie /\w[\w-]*/ for the $name
	    ($name,$rest) = ($name =~ /([^=]+)=(.*)/);
	}

	print STDERR ">> name=$name, rest=$rest\n" if $DEBUG;
	## Make sure that $name is a valid name
	if ($name ne "" and not exists $optHash{$name}) {
	    warn "Invalid option: $name\n";
	    next;
	} 

	## Now that we know the name, go ahead 
	## and get the reference to the object
	my $opt = $optHash{$name};
	
	## If there was an equal sign, then straightforward assignment
	if ($eqsign) {
	    $opt->setvalOf( $rest );
	    next;
	}

	## Check to see if it's a flag;
	unless ($opt->requiresArg) {
	    ## First check for cases: -x+ or -x-
	    ## Hmmm, do we want to consider -x0 and -x1 ??
	    if (defined($rest) && $rest =~ /^([+-])/) {
		($val,$rest) = ($rest =~ /^([+-])(.*)$/);
		$opt->setvalOf( $val );
	    } else {
		$opt->setvalOf;
	    }
	    ## if we had -xyz, and -x was a flag, then
	    ## put "-yz" as the next option 
	    unshift @argv, "-".$rest 
		if (defined($rest) && $rest ne "");

	    next;
	}

	## If it's not a flag
	$rest = shift @argv if (not defined($rest) or $rest eq "");
	$opt->setvalOf($rest);

    }
    push @retargv,@argv if @argv;
    return @retargv;
}
1;
__END__

=head1 Bugs

=over 4

=item *

Given how full-featured and how much more mature the B<Getopts::Long> 
package is, the very existence of this package is arguably a bug.
One might argue that it is right and proper 
that there be more than one way to do it,
but given the availability of the excellent alternative packages 
B<Getopts::Tabular> and B<Getopts::EvaP>, how much do we gain with 
yet another way?

The I<justification> is that
by making it look (to a user of the program) just like the B<C> version of
B<opt>, a program such as B<tkopt> will be useable on both B<C> and
B<Perl> routines.  
It (this package, B<Opt>) does have a few more bells and whistles,
including: support for option files, help strings, and an interactive 
menu, as well as the ability to read default parameters from an
environment variable or a dot file.

The I<real reason> I wrote B<Opt> was to learn
about packages and object-oriented programming in B<Perl>.

=item *

Not all of the B<C-opt> functionality is implemented.

=item *

Some extra functionality, not provided in B<C-opt>, is implemented

=item *

Don't be fooled by the F<Makefile.PL> file; it's just a stub until
I figure out how to make a real one.  However, at the moment, all you
have to do to install is just copy F<Opt.pm> into a 
directory in your B<PERLLIB> list.

In particular 'make test' does not yet work.  However, you can test it 
by hand. just do 'perl t/N.t' where N is the name of the test file;
it it works it should print 'ok' possibly followed by a number.

=back

=head1 Copyright

Copyright (C) 1998, James Theiler; email: jt@lanl.gov

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA  02111-1307, USA.
