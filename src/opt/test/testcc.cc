/* testcc.cc */

#include <iostream.h>
#include <stdio.h>
#include <opt.h>

int month=4;
int day=24;
int year=1993;
int julian=0;
char *who=NULL;
int version=99;

int go(int argc, char **argv)
{
    if (argc>1) {
	cout << 
	"In program: " << argv[0] <<
	"Extra option: " << argv[1] << endl;
    }

    if (optinvoked(&month)) {
	cout << "User set month..." << endl;
    }

    if (month == 9 && day == 11 && year == 1989) {
        printf("Happy birthday, Max\n");
    } else {
        printf("Hello, %s: %4d/%02d/%02d %s\n",(who==NULL ? "world" : who),
               year,month,day,(julian ? "(Julian)" : ""));
    }
    return OPT_OK;
}

int checkyear(void *v)
{
    if (year == 2000) {
        printf("Watch out for that year=2000 bug!\n");
        return OPT_ERROR;
    }
    return OPT_OK;
}
int quit()
{
    printf("Bye...\n");
    return OPT_OK;
}
int write_version(void *v)
{
    /* it's a little hokey that this requires an argument... */
    v = &version; 
    printf("Version %d\n",version);
    optExitNumber(12);
    return OPT_EXIT;
}
int fix_mon(void *v)
{
    int m;
    /* fix whatever int variable v is pointing to */
    m = *((int *)v);
    if (m < 1 || m > 12) 
        m=1;
    *((int *)v) = m;
    return OPT_OK;
} 

int
main(int argc, char **argv)
{
    OptRegister(&month,'m',"month");
    opthook(&month,fix_mon);

    OptRegister(&day,'d',"day","Day");
    opthelp(&day,"Use day of month, should be less than 32");
    
    OptRegister(&year,"year");
    optdescript(&year,"What Year");
    opthook(&year,checkyear);

    OptRegister(&julian,OPT_BOOL,"julian");

    optregp(&who,OPT_STRING,"who","Who to say hello to");

    optexec("version",write_version,"Write version number and exit");

    optEnvVarName( "OPT" );
    optDefaultFile( "testoptrc" ); /* in practice, you do ~/.testoptrc */
    
    optMain(go);
    optQuit(quit);

    opt(&argc,&argv);
    return go(argc,argv);
}



